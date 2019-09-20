
#include "WHPSHttpServer.h"

// WHPSHttpServer* WHPSHttpServer::_http_server = NULL;
std::shared_ptr<WHPSHttpServer> WHPSHttpServer::_http_server;

WHPSHttpServer::WHPSHttpServer()
        : _tcp_server(GetWHPSTcpServer())
        , _worker_thread_pool(atoi(GetWebSourceConfig().get("Server", "workThreads").c_str()))
{
        _worker_thread_pool.start();
        _tcp_server->setNewConnCallback(std::bind(&WHPSHttpServer::onNewConnection, this, std::placeholders::_1));
        // _tcp_server->setNewCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
}

WHPSHttpServer::~WHPSHttpServer()
{

}

/*static */
WHPSHttpServer& WHPSHttpServer::GetInstance()
{
        // if (!_http_server.get())
        if (!_http_server)
        {
                _http_server = std::shared_ptr<WHPSHttpServer>(new WHPSHttpServer());
        }

        return *_http_server.get();
}

/* 启动http服务 */
void WHPSHttpServer::start()
{
        _tcp_server->start();
        _tcp_server->startLoop();
}

void WHPSHttpServer::onNewConnection(sp_TcpSession tcp_session)
{
        this->onNewSession(tcp_session);
}

void WHPSHttpServer::onNewSession(sp_TcpSession tcp_session)
{
        WHPSLogError("WHPSHttpServer::onNewConnection: " + tcp_session->getNetInfo());
        sp_HttpSession http_session(new WHPSHttpSession(tcp_session, _worker_thread_pool));
#if 1
        char addr[1024] = {0};
        sprintf(addr, "%ld", (long)tcp_session.get());
        string str(addr);
        _http_sess_list[tcp_session->getNetInfo() + str] = http_session;
#else
        _http_sess_list[tcp_session->getNetInfo()] = http_session;
#endif
        http_session->setHttpCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
}

void WHPSHttpServer::onNewClose(sp_TcpSession tcp_session)
{
        WHPSLogError("WHPSHttpServer::onNewClose before: %ld", _http_sess_list.size());
#if 1
        char addr[1024] = {0};
        sprintf(addr, "%ld", (long)tcp_session.get());
        string str(addr);
        string key = tcp_session->getNetInfo() + str;
        _http_sess_list.erase(key);
#else
        _http_sess_list.erase(tcp_session->getNetInfo());
#endif
        WHPSLogError("WHPSHttpServer::onNewClose: %ld", _http_sess_list.size());
}
