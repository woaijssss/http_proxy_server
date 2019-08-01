
#include <unistd.h>     // for sleep
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <sys/stat.h>   // for fstat

#include "WHPSHttpSession.h"
#include "util.h"

using namespace std;

static size_t getFileTotalSize(const ifstream& in)
{
        struct stat stat__;
        /* std::filebuf: 流缓冲区用于读取和写入文件。
         *
         */
        auto getFd = [](std::filebuf& fbuff) -> int
        {
                // 继承自std::filebuf，才能使用_M_file protected成员
                struct Helper : public std::filebuf
                {
                        int Handle()
                        { return _M_file.fd(); }
                };
            
                return static_cast<Helper&>(fbuff).Handle();
        };

        int fd = getFd(*in.rdbuf());
        fstat(fd, &stat__);

        return stat__.st_size;
}

/* 测试接口 */
static int load(const string& filename, string& f_buff)
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
#if 0
        std::ifstream in(filename);
        std::stringstream buffer;
        buffer << in.rdbuf();
        f_buff = buffer.str();
#else
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
                 // "Content-Type:text/html\r\n"
                 "Content-Length: " + to_string(length) + "\r\n"
                 "\r\n";
#endif
        f_buff += string(buffer, length);

        delete[] buffer;
#endif

        return 0;
}


#define TestSendMsg(TypeName1, TypeName2) load(TypeName1, TypeName2)         

static void TestSend(const WHPSHttpSession::sp_TcpSession& tcp_session)
{
        string& buff = tcp_session->getBufferIn();
        string test_msg;
        // load("/home/wenhan/server/webResource/html/index.html", test_msg);
#if 1
        int res = TestSendMsg("/home/wenhan/http_proxy_server/webResource/html/index.html", test_msg);
#else
        // int res = TestSendMsg("/home/wenhan/http_proxy_server/webResource/file_test/curl-7.26.0.tar.gz", test_msg);
        int res = TestSendMsg("/home/wenhan/http_proxy_server/webResource/file_test/数据结构(c语言版).pdf", test_msg);
#endif

        if (res < 0)
        {
                string tmp = "404 not found!";
                test_msg = "HTTP/1.1 200 OK \r\n"
                             "Content-Type:text/html\r\n"
                             "Content-Length: " + to_string(tmp.size()) + "\r\n"
                             "\r\n"
                             + tmp;
        }

        cout << "test_msg.size: " << test_msg.size() << endl;
        buff.clear();     // 假设已经处理完毕
        tcp_session->send(test_msg);

        // tcp_session->close();           // 不确定是否需要服务器来释放连接？（目前测试chrome浏览器，必须由服务器释放）
}        

WHPSHttpSession::WHPSHttpSession(const sp_TcpSession& tcp_session)
        : _tcp_session(tcp_session)
        , _http_whps_factory(GetHttpWhpsFactory())      // 获取单例工厂句柄
        , _http_whps(_http_whps_factory->create())      // 获取应用层回调句柄
{
        tcp_session->setHttpMessageCallback(std::bind(&WHPSHttpSession::onHttpMessage, this, std::placeholders::_1));
        tcp_session->setHttpSendCallback(std::bind(&WHPSHttpSession::onHttpSend, this, std::placeholders::_1));
        tcp_session->setHttpCloseCallback(std::bind(&WHPSHttpSession::onHttpClose, this, std::placeholders::_1));
        tcp_session->setHttpErrorCallback(std::bind(&WHPSHttpSession::onHttpError, this, std::placeholders::_1));
}

WHPSHttpSession::~WHPSHttpSession()
{
        cout << __FUNCTION__ << endl;
        if (_http_whps)
        {
                delete _http_whps;
                _http_whps = NULL;  // 后续加到工厂中释放资源
        }
}

const WHPSHttpSession::sp_TcpSession& WHPSHttpSession::getTcpSession() const
{
        return _tcp_session;
}

void WHPSHttpSession::setHttpCloseCallback(HttpSessionCB cb)
{
        _http_closeCB = cb;
}

void WHPSHttpSession::onHttpMessage(const sp_TcpSession& tcp_session)
{
        HttpRequestContext context;
        HttpResponseContext response;
        _http_parser.parseHttpRequest(tcp_session->getBufferIn(), context);     // 解析获取http请求内容

        /* 调用处理部分逻辑 */
        if (context._method == "GET" || context._method == "DELETE")
        {
                _http_whps->doGet(context, response);
        }
        else if (context._method == "POST" || context._method == "PUT")
        {
                _http_whps->doPost(context, response);
        }
        else
        {
                cout << "---WHPSHttpSession::onHttpMessage not support method: [" << context._method << "]" << endl;
        }
        ////////////////////////////


        // tcp_session->setProcessingFlag(true);


        // TestSend(tcp_session);
}

void WHPSHttpSession::onHttpSend(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpSession::onHttpSend" << endl;
        tcp_session->getBufferIn().clear();     // 假设已经处理完毕
}

void WHPSHttpSession::onHttpClose(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpSession::onHttpClose" << endl;
        tcp_session->setProcessingFlag(false);
        _http_closeCB(tcp_session);
}

void WHPSHttpSession::onHttpError(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpSession::onHttpError" << endl;
        tcp_session->setProcessingFlag(false);
        _http_closeCB(tcp_session);
}

void WHPSHttpSession::notifyToClose(const sp_TcpSession& tcp_session)
{
        tcp_session->close();    // 模拟 Connection: close 的情况
}
