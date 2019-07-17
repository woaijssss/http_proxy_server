
#include "WHPSHttpServer.h"

WHPSHttpServer* WHPSHttpServer::_http_server = NULL;

WHPSHttpServer::WHPSHttpServer()
        : _tcp_server(GetWHPSTcpServer())
{
        _tcp_server->setNewConnCallback(std::bind(&WHPSHttpServer::onNewConnection, this, std::placeholders::_1));
        _tcp_server->setNewCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
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

void WHPSHttpServer::onNewConnection(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpServer::onNewConnection: " << tcp_session->getNetInfo() << endl;
}

void WHPSHttpServer::onNewSession(const sp_TcpSession& tcp_session)
{

}

void WHPSHttpServer::onNewClose(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpServer::onNewClose" << endl;
}