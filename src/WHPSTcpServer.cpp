
#include "WHPSTcpServer.h"

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
                return false;
        }

        if (!this->isValid())
        {
                return false;
        }

        _event_chn.setFd(_tcp_socket);
        _event_chn.setEvents(EPOLLIN | EPOLLET);        // 设置接收连接事件，epoll模式为边缘触发

        _event_chn.setReadCallback(std::bind(&WHPSTcpServer::onAccept, this, 0));
        _event_chn.setErrorCallback(std::bind(&WHPSTcpServer::onAccept, this, 1));

        _loop.addEvent(&_event_chn);

        return true;
}

#include <iostream>
using namespace std;
void WHPSTcpServer::onAccept(error_code error)
{
        if (!error)     // 客户端正确连接
        {
                cout << "a client has been connected..." << endl;
        }
        else
        {
                cout << "error_code: " << error << endl;
        }
}
