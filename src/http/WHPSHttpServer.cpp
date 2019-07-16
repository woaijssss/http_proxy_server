
#include "WHPSHttpServer.h"

WHPSHttpServer* WHPSHttpServer::_http_server = NULL;

WHPSHttpServer::WHPSHttpServer()
        : _tcp_server(WHPSTcpServer::Get())
{

}

WHPSHttpServer::~WHPSHttpServer()
{
        cout << "WHPSHttpServer::~WHPSHttpServer" << endl;
}

/*static */
WHPSHttpServer* WHPSHttpServer::Get()
{
        if (!_http_server)
        {
                _http_server = new WHPSHttpServer();
        }

        return _http_server;
}

/* 启动http服务 */
void WHPSHttpServer::start()
{
        _tcp_server->start();
        _tcp_server->startLoop();
}