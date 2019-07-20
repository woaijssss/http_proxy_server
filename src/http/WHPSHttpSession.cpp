
#include "WHPSHttpSession.h"
#include "util.h"

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

}

#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
using namespace std;

/* 测试接口 */
static void load(const string& filename, string& f_buff)
{
#if 0
        std::ifstream t(filename);
        std::stringstream buffer;
        buffer << t.rdbuf();
        f_buff = buffer.str();
#else
        std::ifstream t;  
        int length;  
        t.open(filename, std::ios::in | std::ios::binary);      // open input file
        t.seekg(0, std::ios::end);    // go to the end  
        length = t.tellg();           // report location (this is the length)  
        t.seekg(0, std::ios::beg);    // go back to the beginning  
        char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
        t.read(buffer, length);       // read the whole file into the buffer  

#if 1
        f_buff = "HTTP/1.1 200 OK \r\n"
//                 "Connection:close \r\n"
//                 "Content-Type:application/x-gzip\r\n"
                 "Content-Type:text/html\r\n"
                 "Content-Length: " + to_string(length) + "\r\n"
                 "\r\n";
#endif
        f_buff += string(buffer, length);
        t.close();

        delete[] buffer;
#endif
}

#define TestSendMsg(TypeName1, TypeName2) load(TypeName1, TypeName2)                 

#include <unistd.h>
void WHPSHttpSession::onHttpMessage(const sp_TcpSession& tcp_session)
{
        tcp_session->setProcessingFlag(true);
        string& buff = tcp_session->getBufferIn();
        // cout << "thread_id: " << (unsigned int)std::hash<std::thread::id>()(std::this_thread::get_id()) 
        //     << "----" << getHexString((unsigned char*)buff.c_str(), buff.size()) << endl;

        string test_msg;
        // load("/home/wenhan/server/webResource/html/index.html", test_msg);
#if 1
        TestSendMsg("/home/wenhan/http_proxy_server/webResource/html/index.html", test_msg);
#else
        TestSendMsg("/home/wenhan/http_proxy_server/webResource/file_test/curl-7.26.0.tar.gz", test_msg);
#endif
        cout << "test_msg.size: " << test_msg.size() << endl;
        // sleep(5);
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
        tcp_session->setProcessingFlag(false);
}

void WHPSHttpSession::onHttpError(const sp_TcpSession& tcp_session)
{

}
