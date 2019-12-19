#include <iostream>
using namespace std;

#include "WHPSTcpServer.h" 
#include "WHPSConnSocket.h"
#include "WHPSConfig.h"

// static int g_nthreads = 1;     // 可做成配置(至少是1)

std::shared_ptr<WHPSTcpServer> WHPSTcpServer::m_tcpServer;
// WHPSTcpServer::GC WHPSTcpServer::_gc;

WHPSTcpServer::WHPSTcpServer()
        : ImplSingleton<WHPSTcpServer>(),
          m_loop(),
          m_threadPool(atoi(GetWebSourceConfig().get("Server", "ioThreads").c_str()), m_loop),
          m_tcpSocket(SERVER_MODE),
          m_eventChn(&m_loop)
{

}

WHPSTcpServer::~WHPSTcpServer()
{

}

WHPSTcpServer* WHPSTcpServer::GetInstance()
{
        // if (!m_tcpServer.get())
        if (!m_tcpServer)
        {
                m_tcpServer = std::shared_ptr<WHPSTcpServer>(new WHPSTcpServer());
        }

        return m_tcpServer.get();
}

bool WHPSTcpServer::isValid()
{
        return m_tcpSocket.isValid();
}

WHPSTcpSocket& WHPSTcpServer::getSocket()
{
        return m_tcpSocket;
}

WHPSEpollEventLoop& WHPSTcpServer::loop()
{
        return m_loop;
}

bool WHPSTcpServer::start()
{
        if (m_tcpSocket.init() < 0)     // 启动失败
        {
                WHPSLogError("tcp socket init failed...");

                return false;
        }

        if (!this->isValid())
        {
                WHPSLogError("socket is invalid...");

                return false;
        }

        m_threadPool.start();

        m_eventChn.setFd(m_tcpSocket.get());
        m_eventChn.setEvents(EPOLLIN | EPOLLET);        // 设置接收连接事件，epoll模式为边缘触发

        m_eventChn.setReadCallback(std::bind(&WHPSTcpServer::onNewConnection, this, 0));
        m_eventChn.setErrorCallback(std::bind(&WHPSTcpServer::onNewConnection, this, 1));

        m_loop.addEvent(&m_eventChn);

        return true;
}

#include <unistd.h>
void WHPSTcpServer::startLoop()
{
        m_loop.loop();
}

void WHPSTcpServer::onNewConnection(error_code error)
{
        if (error)      // 客户端连接错误
        {
                WHPSLogError("error_code: %d", error);

                return;
        }

        this->onNewSession();
}

#include <unistd.h>
void WHPSTcpServer::onNewSession()
{
        //循环调用accept，获取所有的建立好连接的客户端fd
        struct sockaddr_in c_addr;
        int fd = -1;

        while ((fd = m_tcpSocket.Accept(c_addr)) > 0)         // 高并发时，可能返回多个连接的事件，因此循环处理
        {
                sp_TcpSession sp_tcp_session(new WHPSTcpSession(m_threadPool.getOneLoop(), fd, c_addr));   // 实例化客户端对象

                // 设置客户端相关参数、回调功能
                //sp_tcp_session->setCleanUpCallback(std::bind(&WHPSTcpServer::onCleanUpResource, this, sp_tcp_session));
                TcpSessionCB cb = std::bind(&WHPSTcpServer::onCleanUpResource, this, std::placeholders::_1);
//                m_tcpSessList[fd] = sp_tcp_session;
                m_tcpSessList[sp_tcp_session->getNetInfo()] = sp_tcp_session;
                sp_tcp_session->setCleanUpCallback(cb);         // 该任务属于线程任务，不属于epoll事件，因此需要设置线程回调函数才能被执行

                m_cbConnect(sp_tcp_session);

                // 将客户端回调任务加入到WHPSEventLoop中
//                sp_tcp_session->addToEventLoop();
                sp_tcp_session->init();
        }
}

void WHPSTcpServer::onCleanUpResource(const sp_TcpSession& sp_tcp_session)
{
        m_tcpSessList.erase(sp_tcp_session->getNetInfo());
        WHPSLogInfo("WHPSTcpServer::onCleanUpResource-----size: %ld", m_tcpSessList.size());

        if (!m_tcpSessList.size())     // 主要是释放map的内存，可能没用（后面换个方法）
        {
                WHPSLogInfo("WHPSTcpServer::onCleanUpResource-----list empty: %ld", m_tcpSessList.size());
                m_tcpSessList.clear();
        }
}

void WHPSTcpServer::setNewConnCallback(cbFunc cb)
{
        m_cbConnect = cb;
}

void WHPSTcpServer::setNewCloseCallback(cbFunc cb)
{
        m_cbClose = cb;
}
