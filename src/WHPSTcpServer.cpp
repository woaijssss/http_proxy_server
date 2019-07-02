
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