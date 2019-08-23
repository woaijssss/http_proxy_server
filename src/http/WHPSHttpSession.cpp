
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

WHPSHttpSession::WHPSHttpSession(const sp_TcpSession _tcp_session)
        : _tcp_session(_tcp_session)
        , _obj_name(GetWebSourceConfig().get("whps", "whps-name"))
        , _whps_static_processor(GetWebSourceConfig().get("StaticResource", "rootDir"))    // 静态资源处理器
        , _http_whps_factory(GetHttpWhpsFactory())          // 获取单例工厂句柄
        , _http_whps(_http_whps_factory->get(_obj_name))    // 获取应用层回调句柄
        , _cb(std::bind(&WHPSHttpSession::TimerCallback, this, std::placeholders::_1))
        , _timer(_cb, NULL, 5*1000)
{
        _writer_func = std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1);
        // 注册http响应消息回调
        _writer.registObj(std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1));
        _tcp_session->setHttpMessageCallback(std::bind(&WHPSHttpSession::onHttpMessage, this));
        _tcp_session->setHttpSendCallback(std::bind(&WHPSHttpSession::onHttpSend, this));
        _tcp_session->setHttpCloseCallback(std::bind(&WHPSHttpSession::onHttpClose, this));
        _tcp_session->setHttpErrorCallback(std::bind(&WHPSHttpSession::onHttpClose, this));

        _timer.start();
}

WHPSHttpSession::~WHPSHttpSession()
{
        // if (_http_whps)
        // {
        //         delete _http_whps;
        //         _http_whps = NULL;  // 后续加到工厂中释放资源
        // }
        cout << "WHPSHttpSession::~WHPSHttpSession()" << endl;
}

const WHPSHttpSession::sp_TcpSession& WHPSHttpSession::getTcpSession() const
{
        return _tcp_session;
}

void WHPSHttpSession::setHttpCloseCallback(HttpSessionCB_ cb)
{
        _http_closeCB = cb;
}

void WHPSHttpSession::onHttpMessage()
{
        cout << "WHPSHttpSession::onHttpMessage: " << _tcp_session->getNetInfo() << endl;
        _tcp_session->setProcessingFlag(true);
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
        _http_closeCB(_tcp_session);
}

void WHPSHttpSession::onHttpError()
{
        cout << "WHPSHttpSession::onHttpError" << endl;
        _http_closeCB(_tcp_session);
}

void WHPSHttpSession::notifyToClose()
{
        /* 当响应头中包含 Connection: close 的时候，需要服务端主动关闭连接
         */
        cout << "WHPSHttpSession::notifyToClose thread: " << std::this_thread::get_id() << endl;
        _timer.stop();
        _tcp_session->close();
        _http_closeCB(_tcp_session);
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
                // this->notifyToClose();
        }
        else
        {
                cout << "WHPSHttpSession::onStaticRequest not support method: [" << request.getMethod() << "]" << endl;
        }
}

void WHPSHttpSession::onDynamicRequest(HttpRequestContext& request, HttpResponseContext& response)
{
#if 1
        _http_whps = _http_whps_factory->get(_obj_name);
        if (!_http_whps)
        {
                cout << "WHPSHttpSession::onHttpMessage whps object is not callable...." << endl;
                this->notifyToClose();

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
                cout << "WHPSHttpSession::onDynamicRequest not support method: [" << method << "]" << endl;
        }
}

void __stdcall WHPSHttpSession::TimerCallback(WHPSTimer& timer)
{
        cout << "WHPSHttpSession::TimerCallback thread: " << std::this_thread::get_id() << endl;
        if (!_tcp_session->getProcessingFlag())
        {
                _tcp_session->getLoop().addTask(std::bind(&WHPSHttpSession::notifyToClose, this)); // 执行清理回调函数
        }
        else
        {
                _tcp_session->setProcessingFlag(false);
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

void WhpsSysResource::doGet(HttpWhpsRequest request, HttpWhpsResponse response)
{
        /* 静态资源自动加载功能（通过配置，获取静态资源路径，并写回给前端）
         */
        string path = _rootPath + request.getUrl();
        string msg;
        this->getResouceFile(path, msg);

        response.getWriter().write(msg);
}

void WhpsSysResource::getResouceFile(const string& path, string& msg)
{
        cout << "WhpsSysResource::getResouceFile path: " << path << endl;
        int res = load(path, msg);

        if (res < 0)
        {
                string tmp = "404 not found!";
                msg = "HTTP/1.1 404 not found \r\n"
                      "Content-Length: " + to_string(tmp.size()) + "\r\n"
                      "\r\n"
                      + tmp;
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

#if 1
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