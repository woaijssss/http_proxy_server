
#include <iostream>
using namespace std;

#include "WHPSTcpServer.h"
#include "WHPSConnSocket.h"

WHPSTcpServer* WHPSTcpServer::_tcp_server = NULL;
WHPSTcpServer::GC WHPSTcpServer::_gc;

WHPSTcpServer::WHPSTcpServer(int maxevents, int timeout)
        : _tcp_socket(SERVER_MODE), _loop(maxevents, timeout)
{
        
}

WHPSTcpServer* WHPSTcpServer::Get(int maxevents, int timeout)
{
        if (!_tcp_server)
        {
                _tcp_server = new WHPSTcpServer(maxevents, timeout);
        }

        return _tcp_server;
}

bool WHPSTcpServer::isValid()
{
        return _tcp_socket.isValid();
}

WHPSTcpSocket& WHPSTcpServer::getSocket()
{
        return _tcp_socket;
}


WHPSEpollEventLoop& WHPSTcpServer::loop()
{
        return _loop;
}

bool WHPSTcpServer::start()
{
        if (_tcp_socket.init() < 0)     // 启动失败
        {
                cout << "tcp socket init failed..." << endl;
                return false;
        }

        if (!this->isValid())
        {
                cout << "socket is invalid..." << endl;
                return false;
        }

        _event_chn.setFd(_tcp_socket.get());
        _event_chn.setEvents(EPOLLIN | EPOLLET);        // 设置接收连接事件，epoll模式为边缘触发

        _event_chn.setReadCallback(std::bind(&WHPSTcpServer::onNewConnection, this, 0));
        _event_chn.setErrorCallback(std::bind(&WHPSTcpServer::onNewConnection, this, 1));

        _loop.addEvent(&_event_chn);

        return true;
}

void WHPSTcpServer::onNewConnection(error_code error)
{
        if (error)      // 客户端连接错误
        {
                cout << "error_code: " << error << endl;
                return;
        }

        cout << "a client has been connected..." << endl;
        this->onNewSession();
}

#include <unistd.h>
void WHPSTcpServer::onNewSession()
{
        //循环调用accept，获取所有的建立好连接的客户端fd
        struct sockaddr_in c_addr;
        int fd = -1;

        while ((fd = _tcp_socket.Accept(c_addr)) > 0)         // 高并发时，可能返回多个连接的事件，因此循环处理
        {
                sp_TcpSession sp_tcp_session(new WHPSTcpSession(_loop, fd, c_addr));   // 实例化客户端对象
                //sp_tcp_session->setCleanUpCallback(std::bind(&WHPSTcpServer::onCleanUpResource, this, sp_tcp_session));
                TcpSessionCB cb = std::bind(&WHPSTcpServer::onCleanUpResource, this, sp_tcp_session);
                sp_tcp_session->setCleanUpCallback(cb);         // 该任务属于线程任务，不属于epoll事件，因此需要设置线程回调函数才能被执行
                
                _tcp_sess_list[fd] = sp_tcp_session;
                sp_tcp_session->addToEventLoop();
                // 设置客户端相关参数、回调功能

                // 将客户端回调任务加入到WHPSEventLoop中
        }
        cout << "------------" << endl;
}

void WHPSTcpServer::onCleanUpResource(sp_TcpSession& sp_tcp_session)
{
        cout << "WHPSTcpServer::onCleanUpResource" << endl;
        _tcp_sess_list.erase(sp_tcp_session->getConn().get());
}