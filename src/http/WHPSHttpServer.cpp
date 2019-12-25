#include "WHPSHttpServer.h"

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

/* static */
WHPSHttpServer& WHPSHttpServer::GetInstance()
{
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
        WHPSLogDebug("WHPSHttpServer::onNewConnection: " + tcp_session->getNetInfo());
        sp_HttpSession http_session(new WHPSHttpSession(tcp_session, m_workerThreadPool));
        m_httpSessList[tcp_session->getNetInfo() + this->getGlobalId((long)tcp_session.get())] = http_session;
        http_session->setHttpCloseCallback(std::bind(&WHPSHttpServer::onNewClose, this, std::placeholders::_1));
        http_session->init();
}

void WHPSHttpServer::onNewClose(sp_TcpSession tcp_session)
{
        WHPSLogDebug("WHPSHttpServer::onNewClose before: %ld", m_httpSessList.size());
        string key = tcp_session->getNetInfo() + this->getGlobalId((long)tcp_session.get());
        m_httpSessList.erase(key);
        WHPSLogDebug("WHPSHttpServer::onNewClose: %ld", m_httpSessList.size());
}

string WHPSHttpServer::getGlobalId(const long& key)
{
        // 9223372036854775807  long的最大值
        char addr[32] = { 0 };
        sprintf(addr, "%ld", key);

        return string(addr);
}
