
#include <unistd.h>     // for sleep
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <sys/stat.h>   // for fstat

#include "WHPSHttpSession.h"
#include "util.h"
#include "WHPSConfig.h"

#include "WhpsObject.h"

using namespace std;     

WHPSHttpSession::WHPSHttpSession(const sp_TcpSession _tcp_session, WHPSWorkerThreadPool& worker_thread_pool)
        : _tcp_session(_tcp_session)
	, _http_closeCB(nullptr)
        , _obj_name(GetWebSourceConfig().get("whps", "whps-name"))
        , _whps_static_processor(GetWebSourceConfig().get("StaticResource", "rootDir"))    // 静态资源处理器
        , _http_whps_factory(GetHttpWhpsFactory())          // 获取单例工厂句柄
        , _http_whps(_http_whps_factory->get(_obj_name))    // 获取应用层回调句柄
        , _cb(std::bind(&WHPSHttpSession::TimerCallback, this, std::placeholders::_1))
        , _timer(_cb, NULL, atoi(GetWebSourceConfig().get("Server", "httpTimeOut").c_str()))
        , _conn_status(INIT)
        , _worker_thread_pool(worker_thread_pool)
{
         _writer_func = std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1);
        // 注册http响应消息回调
        _writer.registObj(std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1));
        _tcp_session->setHttpMessageCallback(std::bind(&WHPSHttpSession::onHttpMessage, this));
        _tcp_session->setHttpSendCallback(std::bind(&WHPSHttpSession::onHttpSend, this));
        _tcp_session->setHttpCloseCallback(std::bind(&WHPSHttpSession::onHttpClose, this));
        _tcp_session->setHttpErrorCallback(std::bind(&WHPSHttpSession::onHttpClose, this));
}

WHPSHttpSession::~WHPSHttpSession()
{
        // if (_http_whps)
        // {
        //         delete _http_whps;
        //         _http_whps = NULL;  // 后续加到工厂中释放资源
        // }
        _timer.stop();
}

void WHPSHttpSession::init()
{
        _timer.start();

}

const WHPSHttpSession::sp_TcpSession& WHPSHttpSession::getTcpSession() const
{
        return _tcp_session;
}

void WHPSHttpSession::setHttpCloseCallback(HttpSessionCB_ cb)
{
	std::lock_guard<std::mutex> lock(_mutex);	
        _http_closeCB = cb;
}

void WHPSHttpSession::onHttpMessage()
{
        if (this->getConnStatus() > PROCESSING)
        {
                WHPSLogWarn("connectStatus flag is bigger than CLOSING...: " + _tcp_session->getNetInfo());
                this->closeAll();
                return;
        }

//        std::lock_guard<std::mutex> lock(_mutex);
        this->setConnStatus(PROCESSING);
        HttpRequestContext request;
        HttpResponseContext response(_writer_func);
        _http_parser.parseHttpRequest(_tcp_session->getBufferIn(), request);     // 解析获取http请求内容

        /* 增加判断http粘包问题后，从原始包中删除一个http整包，保留剩下的bufferin */
        _tcp_session->getBufferIn().clear();     // 假设已经处理完毕
        /***************/

        this->onCallback(request, response);    // 调用消息回调处理程序
}

void WHPSHttpSession::onHttpSend()
{
        _tcp_session->getBufferIn().clear();     // 假设已经处理完毕
}

void WHPSHttpSession::onHttpClose()
{
        WHPSLogWarn("WHPSHttpSession::onHttpClose: " + _tcp_session->getNetInfo());
        this->closeAll();
}

void WHPSHttpSession::onHttpError()
{
        WHPSLogWarn("WHPSHttpSession::onHttpError: " + _tcp_session->getNetInfo());
        this->closeAll();
}

void WHPSHttpSession::closeAll()
{
        std::lock_guard<std::mutex> lock(_mutex);

        if (this->getConnStatus() > CLOSING)
        {
                return;
        }

        _timer.stop();
        this->setConnStatus(DISCONNECTED);
        _tcp_session->closeSession();
        _worker_thread_pool._task.addTask(std::bind(&WHPSHttpSession::notifyToClose, shared_from_this()));
}

void WHPSHttpSession::notifyToClose()
{
//        std::lock_guard<std::mutex> lock(_mutex);
        /* 当响应头中包含 Connection: close 的时候，需要服务端主动关闭连接
         */
        {
                std::lock_guard<std::mutex> lock(_mutex);
                if (this->getConnStatus() == STOPPED)
                {
                        return;
                }

                this->setConnStatus(STOPPED);
        }

        _timer.stop();
        _tcp_session->release();        // 释放tcp层资源

        {
                /* 该锁如果放到函数开始的位置，会导致“交叉锁”，
                 * 造成两个线程池相互死锁
                 */
                std::lock_guard<std::mutex> lock(_mutex);
                if (_http_closeCB)
                {
                        _http_closeCB(_tcp_session);
                }
        }
}

void WHPSHttpSession::TimerCallback(WHPSTimer& timer)
{
        std::lock_guard<std::mutex> lock(_mutex);

        if (this->getConnStatus() < CLOSING)
        {
                this->setConnStatus(CLOSING);
        }
        else if (this->getConnStatus() == CLOSING)
        {
                WHPSLogInfo("add to task queue and stop the timer: %ld", timer.id());
                timer.stop();
                this->setConnStatus(DISCONNECTED);
                _tcp_session->closeSession();       // 马上关闭对外连接，并停止事件触发

                try
                {
                        _worker_thread_pool._task.addTask(std::bind(&WHPSHttpSession::notifyToClose, shared_from_this()));
                }
                catch (exception& e)
                {
                        WHPSLogWarn("WHPSHttpSession::TimerCallback exception: %s", e.what());
                }
        }
        else
        {
                timer.stop();
        }
}

void WHPSHttpSession::sendHttpMessage(const string& msg)
{
        _tcp_session->send(msg);
}

void WHPSHttpSession::onCallback(HttpRequestContext& request, HttpResponseContext& response)
{
        /* 调用处理部分逻辑 */
        if (request.getFlag())      // 静态资源
        {
                this->onStaticRequest(request, response);
        }
        else
        {
                this->onDynamicRequest(request, response);
        }
}

void WHPSHttpSession::onStaticRequest(HttpRequestContext& request, HttpResponseContext& response)
{
        /* 框架请求静态资源，仅支持 GET 方法获取
         * 不支持也没必要支持 POST、DELETE
         */
        if (request.getMethod() == "GET")
        {
                _whps_static_processor.doGet(request, response);    // 通过静态资源处理器直接返回
        }
        else if (request.getMethod() == "PUT")
        {
                WHPSLogInfo("WHPSHttpSession::onStaticRequest method-[%s] 用于上传静态资源", request.getMethod());
        }
        else
        {
                WHPSLogWarn("WHPSHttpSession::onStaticRequest not support method: [" + request.getMethod() + "]");
        }
}

void WHPSHttpSession::onDynamicRequest(HttpRequestContext& request, HttpResponseContext& response)
{
#if 1
        _http_whps = _http_whps_factory->get(_obj_name);

        if (!_http_whps)
        {
                WHPSLogWarn("WHPSHttpSession::onHttpMessage whps object is not callable....");
                this->closeAll();
//                this->notifyToClose();

                return;
        }
#endif

        const string& method = request.getMethod();

        if (method == "GET" || method == "DELETE")
        {
                _http_whps->doGet(request, response);
        }
        else if (method == "POST" || method == "PUT")
        {
                _http_whps->doPost(request, response);
        }
        else
        {
                WHPSLogWarn("WHPSHttpSession::onDynamicRequest not support method: [" + method + "]");
        }
}

/*  */
static int load(const string& filename, string& f_buff);

WhpsSysResource::WhpsSysResource(const string& rootPath)
        : _rootPath(rootPath)
{

}

WhpsSysResource::~WhpsSysResource()
{

}

void WhpsSysResource::doGet(HttpWhpsRequest& request, HttpWhpsResponse& response)
{
        /* 静态资源自动加载功能（通过配置，获取静态资源路径，并写回给前端）
         */
        string path = _rootPath + request.getUrl();
        string msg;
        this->getResouceFile(path, response, msg);

        response.getWriter().write(msg);
}

void WhpsSysResource::getResouceFile(const string& path, HttpWhpsResponse& response, string& msg)
{
        WHPSLogInfo("WhpsSysResource::getResouceFile path: %s", path.c_str());
        int res = load(path, msg);

        if (res < 0)
        {
                msg = "404 not found!";
                response.setError(404, "Not Found");
        }
}

/* 获取文件大小 */
static size_t getFileTotalSize(const ifstream& in)
{
/*
        struct stat {
               dev_t     st_dev;     // ID of device containing file 
               ino_t     st_ino;     // inode number 
               mode_t    st_mode;    // protection 
               nlink_t   st_nlink;   // number of hard links 
               uid_t     st_uid;     // user ID of owner 
               gid_t     st_gid;     // group ID of owner 
               dev_t     st_rdev;    // device ID (if special file) 
               off_t     st_size;    // total size, in bytes 
               blksize_t st_blksize; // blocksize for file system I/O 
               blkcnt_t  st_blocks;  // number of 512B blocks allocated 
               time_t    st_atime;   // time of last access 
               time_t    st_mtime;   // time of last modification 
               time_t    st_ctime;   // time of last status change 
        };
 */
        struct stat stat__;

        /* std::filebuf: 流缓冲区用于读取和写入文件。 */
        auto getFd = [](std::filebuf& fbuff) -> int
        {
                // 继承自std::filebuf，才能使用_M_file protected成员
                struct Helper : public std::filebuf
                {
                        int Handle()
                        {
                                return _M_file.fd(); 
                        }
                };
            
                return static_cast<Helper&>(fbuff).Handle();
        };

        int fd = getFd(*in.rdbuf());
        fstat(fd, &stat__);

        return stat__.st_size;
}

/* 读取指定文件到缓冲buff */
int load(const string& filename, string& f_buff)
{
        std::ifstream in;   
        in.open(filename, std::ios::in | std::ios::binary);      // open input file

        if (!in)
        {
                return -1;
        }

        
        size_t length = getFileTotalSize(in);

        char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
        in.read(buffer, length);       // read the whole file into the buffer  
        in.close();

#if 0
        f_buff = "HTTP/1.1 200 OK \r\n"
//                 "Connection:close \r\n"
                // "Content-Type:application/x-gzip\r\n"
//                  "Content-Type:text/html\r\n"
                 "Content-Length: " + to_string(length) + "\r\n"
                 "\r\n";
#endif
        f_buff += string(buffer, length);
        delete[] buffer;

        return 0;
}
