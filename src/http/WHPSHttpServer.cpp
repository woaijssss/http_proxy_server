
#include "WHPSHttpServer.h"

// WHPSHttpServer* WHPSHttpServer::_http_server = NULL;
std::shared_ptr<WHPSHttpServer> WHPSHttpServer::_http_server;

WHPSHttpServer::WHPSHttpServer()
        : _tcp_server(GetWHPSTcpServer())
{
        _tcp_server->setNewConnCallback(std::bind(&WHPSHttpServer::onNewConnection, this, std::placeholders::_1));
        // _tcp_server->setNewCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
}

WHPSHttpServer::~WHPSHttpServer()
{
        cout << "WHPSHttpServer::~WHPSHttpServer" << endl;
}

/*static */
WHPSHttpServer* WHPSHttpServer::Get()
{
        if (!_http_server.get())
        {
                _http_server = std::shared_ptr<WHPSHttpServer>(new WHPSHttpServer());
        }

        return _http_server.get();
}

/* 启动http服务 */
void WHPSHttpServer::start()
{
        _tcp_server->start();
        _tcp_server->startLoop();
}

void WHPSHttpServer::onNewConnection(const sp_TcpSession& tcp_session)
{
        this->onNewSession(tcp_session);
}

void WHPSHttpServer::onNewSession(const sp_TcpSession& tcp_session)
{
        cout << "WHPSHttpServer::onNewConnection: " << tcp_session->getNetInfo() << endl;
        sp_HttpSession http_session(new WHPSHttpSession(tcp_session));
        http_session->setHttpCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
        _http_sess_list[tcp_session->getNetInfo()] = http_session;
}

void WHPSHttpServer::onNewClose(const sp_TcpSession& tcp_session)
{
        _http_sess_list.erase(tcp_session->getNetInfo());
        cout << "WHPSHttpServer::onNewClose-----size: " << _http_sess_list.size() << endl;
}