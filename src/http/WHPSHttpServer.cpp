#include "WHPSHttpServer.h"

// WHPSHttpServer* WHPSHttpServer::m_httpServer = NULL;
std::shared_ptr<WHPSHttpServer> WHPSHttpServer::m_httpServer;

WHPSHttpServer::WHPSHttpServer()
        : m_tcpServer(GetWHPSTcpServer()),
          m_workerThreadPool(atoi(GetWebSourceConfig().get("Server", "workThreads").c_str()))
{
        m_workerThreadPool.start();
        m_tcpServer->setNewConnCallback(std::bind(&WHPSHttpServer::onNewConnection, this, std::placeholders::_1));
        // m_tcpServer->setNewCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
}

WHPSHttpServer::~WHPSHttpServer()
{

}

/*static */
WHPSHttpServer& WHPSHttpServer::GetInstance()
{
        // if (!m_httpServer.get())
        if (!m_httpServer)
        {
                m_httpServer = std::shared_ptr<WHPSHttpServer>(new WHPSHttpServer());
        }

        return *m_httpServer.get();
}

/* 启动http服务 */
void WHPSHttpServer::start()
{
        m_tcpServer->start();
        m_tcpServer->startLoop();
}

void WHPSHttpServer::onNewConnection(sp_TcpSession tcp_session)
{
        this->onNewSession(tcp_session);
}

void WHPSHttpServer::onNewSession(sp_TcpSession tcp_session)
{
        WHPSLogError("WHPSHttpServer::onNewConnection: " + tcp_session->getNetInfo());
        sp_HttpSession http_session(new WHPSHttpSession(tcp_session, m_workerThreadPool));
#if 1
        char addr[1024] = { 0 };
        sprintf(addr, "%ld", (long) tcp_session.get());
        string str(addr);
        m_httpSessList[tcp_session->getNetInfo() + str] = http_session;
#else
        m_httpSessList[tcp_session->getNetInfo()] = http_session;
#endif
        http_session->setHttpCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
        http_session->init();
}

void WHPSHttpServer::onNewClose(sp_TcpSession tcp_session)
{
        WHPSLogError("WHPSHttpServer::onNewClose before: %ld", m_httpSessList.size());
#if 1
        char addr[1024] = { 0 };
        sprintf(addr, "%ld", (long) tcp_session.get());
        string str(addr);
        string key = tcp_session->getNetInfo() + str;
        m_httpSessList.erase(key);
#else
        m_httpSessList.erase(tcp_session->getNetInfo());
#endif
        WHPSLogError("WHPSHttpServer::onNewClose: %ld", m_httpSessList.size());
}
