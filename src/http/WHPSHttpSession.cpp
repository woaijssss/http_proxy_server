
#include <unistd.h>     // for sleep
#include <iostream>
#include <thread>

#include "WHPSHttpSession.h"
#include "util.h"

using namespace std;     

WHPSHttpSession::WHPSHttpSession(const sp_TcpSession& tcp_session)
        : _tcp_session(tcp_session)
        , _http_whps_factory(GetHttpWhpsFactory())      // 获取单例工厂句柄
        , _http_whps(_http_whps_factory->create())      // 获取应用层回调句柄
{
        _writer_func = std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1);
        // 注册http响应消息回调
        _writer.registObj(std::bind(&WHPSHttpSession::sendHttpMessage, this, std::placeholders::_1));
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
        cout << "WHPSHttpSession::onHttpMessage" << endl;
        HttpRequestContext context;
        HttpResponseContext response(_writer_func);
        cout << "_____________________________--" << endl;
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
//        sleep(5);
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
        /* 当响应头中包含 Connection: close 的时候，需要服务端主动关闭连接
         */
        tcp_session->close();
}

void WHPSHttpSession::sendHttpMessage(const string& msg)
{
//        cout << "msg: " << msg << endl;

        _tcp_session->send(msg);
//        _tcp_session->getBufferIn().clear();    // 测试
}
