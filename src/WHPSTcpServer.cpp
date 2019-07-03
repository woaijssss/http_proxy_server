
#include "WHPSTcpServer.h"

WHPSTcpServer* WHPSTcpServer::_tcp_server = NULL;
WHPSTcpServer::GC WHPSTcpServer::_gc;

WHPSTcpServer::WHPSTcpServer()
        : _tcp_socket(SERVER_MODE)
{
        _tcp_socket.init();
}

WHPSTcpServer* WHPSTcpServer::Get()
{
        if (!_tcp_server)
        {
                _tcp_server = new WHPSTcpServer();
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

bool WHPSTcpServer::start()
{
        if (!this->isValid())
        {
                return false;
        }

        _event_chn.setFd(_tcp_socket);
        _event_chn.setEvents(EPOLLIN | EPOLLET);        // 设置接收连接事件，epoll模式为边缘触发

        return true;
}
