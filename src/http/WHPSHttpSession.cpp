
#include <unistd.h>     // for sleep
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>

#include "WHPSHttpSession.h"
#include "util.h"

using namespace std;

/* 测试接口 */
static int load(const string& filename, string& f_buff)
{
#if 0
        std::ifstream in(filename);
        std::stringstream buffer;
        buffer << in.rdbuf();
        f_buff = buffer.str();
#else
        std::ifstream in;  
        int length;  
        in.open(filename, std::ios::in | std::ios::binary);      // open input file

        if (!in)
        {
                return -1;
        }

        in.seekg(0, std::ios::end);    // go to the end  
        length = in.tellg();           // report location (this is the length)  
        in.seekg(0, std::ios::beg);    // go back to the beginning  
        char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
        in.read(buffer, length);       // read the whole file into the buffer  
        in.close();

#if 1
        f_buff = "HTTP/1.1 200 OK \r\n"
//                 "Connection:close \r\n"
                "Content-Type:application/x-gzip\r\n"
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


WHPSHttpSession::WHPSHttpSession(const sp_TcpSession& tcp_session)
        : _tcp_session(tcp_session)
{
        tcp_session->setHttpMessageCallback(std::bind(&WHPSHttpSession::onHttpMessage, this, std::placeholders::_1));
        tcp_session->setHttpSendCallback(std::bind(&WHPSHttpSession::onHttpSend, this, std::placeholders::_1));
        tcp_session->setHttpCloseCallback(std::bind(&WHPSHttpSession::onHttpClose, this, std::placeholders::_1));
        tcp_session->setHttpErrorCallback(std::bind(&WHPSHttpSession::onHttpError, this, std::placeholders::_1));
}

WHPSHttpSession::~WHPSHttpSession()
{
        cout << __FUNCTION__ << endl;
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
        tcp_session->setProcessingFlag(true);
        string& buff = tcp_session->getBufferIn();

        string test_msg;
        // load("/home/wenhan/server/webResource/html/index.html", test_msg);
#if 0
        int res = TestSendMsg("/home/wenhan/http_proxy_server/webResource/html/index.html", test_msg);
#else
        int res = TestSendMsg("/home/wenhan/http_proxy_server/webResource/file_test/curl-7.26.0.tar.gz1", test_msg);
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
        tcp_session->getBufferIn().clear();     // 假设已经处理完毕
        tcp_session->send(test_msg);

        // tcp_session->close();           // 不确定是否需要服务器来释放连接？（目前测试chrome浏览器，必须由服务器释放）
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
