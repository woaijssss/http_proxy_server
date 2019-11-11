#include <iostream>
#include <unistd.h>
using namespace std;

#include "WHPSTcpSession.h"
#include "util.h"

#define C_INVALID_PORT	-1

WHPSTcpSession::WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr)
        : std::enable_shared_from_this<WHPSTcpSession>(),
          m_cAddr(c_addr),
          m_loop(loop),
          m_connSock(fd),
          m_eventChn(&m_loop),
          m_baseEvents(EPOLLIN | EPOLLPRI),
          m_cbCleanup(nullptr),
          m_clientPort(C_INVALID_PORT),
          m_isConnect(true),
          m_isStop(false),
          m_isWait(false),
          m_httpOnMessage(nullptr),
          m_httpOnSend(nullptr),
          m_httpOnClose(nullptr),
          m_httpOnError(nullptr)
{
        this->getEndpointInfo();
}

WHPSTcpSession::~WHPSTcpSession()
{

}

void WHPSTcpSession::init()
{
        m_connSock.setOption();
        /* 每个客户端的socket要设置成非阻塞，否则在read或write会使线程阻塞，无法实现异步和线程复用 */
        m_connSock.setNonblock();

        m_eventChn.setFd(m_connSock.get());
        // m_eventChn.setEvents(EPOLLIN | EPOLLPRI | EPOLLOUT); // 不能设置EPOLLOUT，否则客户端连接，会频繁调用onNewWrite()
        m_eventChn.setEvents(m_baseEvents);        // 设置接收连接事件，epoll模式为边缘触发

        m_eventChn.setReadCallback(std::bind(&WHPSTcpSession::onNewRead, this, 0)); // 注册数据接收回调函数
        m_eventChn.setWriteCallback(std::bind(&WHPSTcpSession::onNewWrite, this, 0)); // 注册数据发送回调函数
        m_eventChn.setCloseCallback(std::bind(&WHPSTcpSession::onNewClose, this, 0)); // 注册连接关闭回调函数
        m_eventChn.setErrorCallback(std::bind(&WHPSTcpSession::onNewError, this, 0)); // 注册异常错误回调函数

        this->addToEventLoop();
        // 还需要注册发送数据和超时回调
}

void WHPSTcpSession::getEndpointInfo()
{
        struct sockaddr_in sa;
        socklen_t len = sizeof(sa);
        if (!getpeername(m_connSock.get(), (struct sockaddr *) &sa, &len))
        {
                m_clientIp = string(inet_ntoa(sa.sin_addr));
                m_clientPort = ntohs(sa.sin_port);
                m_netInfo = m_clientIp + ":" + to_string(m_clientPort);
        }

        WHPSLogInfo("a client has been connected: " + m_netInfo);
}

const std::string& WHPSTcpSession::getIp() const
{
        return m_clientIp;
}

const int& WHPSTcpSession::getPort() const
{
        return m_clientPort;
}

const std::string& WHPSTcpSession::getNetInfo() const
{
        return m_netInfo;
}

bool WHPSTcpSession::isValid()
{
        return m_connSock.isValid();
}

const int& WHPSTcpSession::get() const
{
        return m_connSock.get();
}

WHPSConnSocket& WHPSTcpSession::getConn()
{
        return m_connSock;
}

void WHPSTcpSession::closeSession()
{
        /* 该函数主要是关闭所有对外接口方法，包括但不限于：
         * （1）socket句柄；
         * （2）epoll事件循环；
         * 并设置标志位，防止多次进入造成资源二次释放
         */
        if (!this->getConnectFlag())
        {
                return;
        }

        this->setConnectFlag();
        this->delFromEventLoop();
//        m_eventChn.stop();     // 直接停止调用回调

        {
                // 防止 WHPSEventLoop 对象调用回调函数时，析构 WHPSEventHandler 对象
//        		std::lock_guard<std::mutex> lock(m_loop.getMutex());
                m_eventChn.stop();     // 直接停止调用回调
        }

        m_isWait = true;
        // m_loop.addTask(std::bind(m_cbCleanup, shared_from_this())); // 执行清理回调函数
        //m_bufferIn.clear();
        //m_bufferOut.clear();
        m_httpOnMessage = nullptr;
        m_httpOnSend = nullptr;
        m_httpOnClose = nullptr;
        m_httpOnError = nullptr;
}

void WHPSTcpSession::addToEventLoop()
{
        m_loop.addEvent(&m_eventChn);    // 所有请求都由主线程处理(后续要修改成多线程)
}

void WHPSTcpSession::delFromEventLoop()
{
        m_eventChn.setEvents(m_baseEvents);
        m_loop.updateEvent(&m_eventChn);
        m_loop.delEvent(&m_eventChn);
}

void WHPSTcpSession::setCleanUpCallback(TcpSessionCB& cb)
{
        m_cbCleanup = cb;
}

void WHPSTcpSession::onCall(httpCB cb)
{
#if 0
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_isConnect)
        {
                return;
        }
#endif

        std::lock_guard<std::mutex> lock(m_mutex);
        if (!cb)
        {
                return;
        }

        cb();
}

void WHPSTcpSession::release()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_isStop)
        {
                return;
        }

        m_isStop = true;
        m_connSock.close();

        try
        {
                if (m_cbCleanup)
                {
                        m_cbCleanup(shared_from_this());
                        m_cbCleanup = nullptr;
                }
        } catch (exception& e)
        {
                WHPSLogWarn("WHPSTcpSession::release exception: %s", e.what());
        }
}

void WHPSTcpSession::send(const std::string& msg)
{
        m_bufferOut = msg;
        int res = this->sendTcpMessage(m_bufferOut);    // 需要判别发送的结果，从而决定要不要关闭连接

        if (res > 0)    // 正确发送数据
        {
                events_t events = m_eventChn.getEvents();

                if (m_bufferOut.size())     // 还有数据未发送(可能是tcp缓冲区占满导致)
                {
                        events = events | EPOLLOUT;         // 设置写监听
                } else        // 数据发送完毕
                {
                        events = m_baseEvents;      // 数据发送完毕，无需继续监听写操作
//                        m_httpOnSend(shared_from_this());
//                        this->onCall(m_httpOnSend);

                        if (m_isWait)
                        {
                                this->onNewClose(0);
                        }
                }

                m_eventChn.setEvents(events);
                m_loop.updateEvent(&m_eventChn);
        } else if (res == 0)    // 数据发送异常
        {
                onNewClose(-1); // 关闭连接
        } else
        {
                onNewError(-1); // 错误处理，释放资源
        }
}

int WHPSTcpSession::sendTcpMessage(std::string& buffer_out)
{
        /* 当前发送数据，是一个线程将_buffer_out中的所有数据全部发送
         * 后续是否需要改成每次只发送一比数据，之后的数据靠epoll事件触发？
         */
        int res = -1;

        long bytes_transferred = 0;
        while (true)
        {
                // int w_nbytes = write(m_connSock.get(), buffer_out.c_str(), buffer_out.size());
                /* 写入数据可能产生 SIGPIPE 信号
                 * 对发送函数设置 MSG_NOSIGNAL，可忽略此信号，不至于进程退出
                 */
                int w_nbytes = ::send(m_connSock.get(), buffer_out.c_str(), buffer_out.size(), MSG_NOSIGNAL);
                bytes_transferred += w_nbytes;

                if (w_nbytes > 0)   // 该笔数据已发送
                {
                        buffer_out.erase(0, w_nbytes); // 清除缓冲区中已发送的数据(不适用于重发的情况)

                        if (m_bufferOut.size() == 0)
                        {
                                res = bytes_transferred;
                                break;
                        }

                } else if (w_nbytes == 0)
                {
                        if (errno == EAGAIN)    // 数据正确的发送完成，再次调用的返回结果
                        {
                                res = bytes_transferred;
                        }

                        break;
                } else    // 写数据异常
                {
                        if (errno == EAGAIN)    // tcp发送缓冲区满了，下次继续发送
                        {
                                /* 这里统一返回1！
                                 *  若返回bytes_transferred，当写一次出错时，同时errno是EAGAIN，不应关闭
                                 *  但实际的bytes_transferred结果为-1，会导致异常关闭。
                                 */
                                res = 1;
                        } else if (errno == EPIPE) // 客户端已经close，并发了RST，继续wirte会报EPIPE，返回0，表示close
                        {
                                // pass
                        } else if (errno == EINTR) // 中断，write()会返回-1，同时置errno为EINTR
                        {
                                // pass
                        } else
                        {
//                                "unknow errno type..." << endl;
                        }

                        break;
                }
        }

        return res;
}

#include <thread>
void WHPSTcpSession::onNewRead(error_code error)
{
        int res = this->readTcpMessage(m_bufferIn);

        if (res > 0)
        {
                // 进行业务层的数据数据(tcp数据处理)
                // 异步回调方式，epoll线程跟业务线程分离
//                m_httpOnMessage(shared_from_this());
                this->onCall(m_httpOnMessage);

#if 0           // 测试socket关闭后，再发送数据的错误处理
                TestSend(this);
                // 测试发送接口修改成通过向任务队列抛任务的方式，由不同的线程异步执行
#endif
        } else if (res == 0)
        {
                onNewClose(-1); // 关闭连接
        } else
        {
                onNewError(-1); // 错误处理，释放资源
        }
}

int WHPSTcpSession::readTcpMessage(std::string& buffer_in)
{
        int res = -1;
        int bytes_transferred = 0;

        while (true)
        {
                char buffer[1024];
                // int r_nbyte = read(m_connSock.get(), buffer, 1);
                /* 参考nginx实现方式
                 */
                // int r_nbyte = recvfrom(m_connSock.get(), buffer, 1024, 0, NULL, NULL);
                int r_nbyte = recv(m_connSock.get(), buffer, 1024, 0);

                if (r_nbyte > 0)    // 正常读取数据
                {
                        m_bufferIn.append(buffer, r_nbyte);     // 每次追加写入数据
                        bytes_transferred += r_nbyte;
                } else if (r_nbyte == 0)    // 客户端关闭socket，FIN包
                {
                        res = 0;

                        break;
                } else    // 读数据异常(-1)
                {
                        // 异步时，当缓冲区无数据时，read会返回-1,即：读完了
                        /* 在非阻塞模式下调用了阻塞操作，在该操作没有完成就返回这个错误，
                         * 这个错误不会破坏socket的同步，下次循环接着recv就可以。
                         */
                        if (errno == EAGAIN)
                        {
                                // pass
                                // res = bytes_transferred;
                                res = (bytes_transferred == 0) ? 1 : bytes_transferred;
                        } else if (errno == EINTR) // 中断，read()会返回-1，同时置errno为EINTR
                        {
                                res = -1;
                        }

                        break;
                }
        }

        return res;
}

void WHPSTcpSession::onNewWrite(error_code error)
{
        int res = this->sendTcpMessage(m_bufferOut);    // 需要判别发送的结果，从而决定要不要关闭连接

        if (res > 0)    // 正确发送数据
        {
                events_t events = m_eventChn.getEvents();

                if (m_bufferOut.size())     // 还有数据未发送(可能是tcp缓冲区占满导致)
                {
                        events = events | EPOLLOUT;         // 设置写监听
                } else        // 数据发送完毕
                {
                        events = m_baseEvents;      // 数据发送完毕，无需继续监听写操作
//                        m_httpOnSend(shared_from_this());
                        this->onCall(m_httpOnSend);
                        if (m_isWait)
                        {
                                this->onNewClose(0);
                        }
                }

                m_eventChn.setEvents(events);
                m_loop.updateEvent(&m_eventChn);
        } else if (res == 0)    // 数据发送异常
        {
                onNewClose(-1); // 关闭连接
        } else
        {
                onNewError(-1); // 错误处理，释放资源
        }
}

void WHPSTcpSession::onNewClose(error_code error)
{
        /* 防止陷入死循环
         * 正常执行完_cb_cleanup,清除对应的智能指针占用,可以使其引用计数降为0,从而正确退出session.
         * 而因为addTask传递_cb_cleanup,所携带shared_from_this(),会使指向当前对象的shared_ptr引用计数加1,
         * 因此,再次引用时,无需执行此函数,直接返回,使引用计数降为0即可.
         */
        if (!this->getConnectFlag())
        {
                return;
        }

        /* 关闭数据需要做两件事情
         *      （1）处理剩余数据(接收_buffer_in/发送_buffer_out)————(暂不考虑，后续扩展)
         *      （2）关闭socket
         *      （3）通知主socket对象和EventLoop对象删除该句柄资源
         */
// if (m_bufferIn.size() || m_bufferOut.size() || _is_processing)
        if (m_bufferIn.size() || m_bufferOut.size())
        {
                if (m_bufferIn.size())
                {
//                        m_httpOnMessage(shared_from_this());
                        this->onCall(m_httpOnMessage);
                }
        } else
        {
                if (m_isWait)   // 如果服务端主动关闭，需要有延迟来保证对端接收数据完毕
                {
                        delayMs(0);
                }

                this->onCall(m_httpOnClose);
                // this->release();
        }
}

void WHPSTcpSession::onNewError(error_code error)
{
        if (!this->getConnectFlag())
        {
                return;
        }
        /* 关闭数据需要做两件事情
         *      （1）关闭socket
         *      （2）通知主socket对象和EventLoop对象删除该句柄资源
         */
        this->onCall(m_httpOnError);
        // this->release();
}

void WHPSTcpSession::setHttpMessageCallback(httpCB cb)
{
        m_httpOnMessage = cb;
}

void WHPSTcpSession::setHttpSendCallback(httpCB cb)
{
        m_httpOnSend = cb;
}

void WHPSTcpSession::setHttpCloseCallback(httpCB cb)
{
        m_httpOnClose = cb;
}

void WHPSTcpSession::setHttpErrorCallback(httpCB cb)
{
        m_httpOnError = cb;
}

