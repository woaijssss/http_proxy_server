
#include <iostream>
#include <unistd.h>
using namespace std;

#include "WHPSTcpSession.h"
#include "util.h"

WHPSTcpSession::WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr)
        : std::enable_shared_from_this<WHPSTcpSession>()
          , _c_addr(c_addr), _loop(loop), _conn_sock(fd)
{
        /* 每个客户端的socket要设置成非阻塞，否则在read或write会使线程阻塞，无法实现异步和线程复用 */
        _conn_sock.setNonblock();

        _event_chn.setFd(_conn_sock.get());
        _event_chn.setEvents(EPOLLIN | EPOLLPRI);        // 设置接收连接事件，epoll模式为边缘触发

        _event_chn.setReadCallback(std::bind(&WHPSTcpSession::onNewRead, this, 0));     // 注册数据接收回调
        _event_chn.setCloseCallback(std::bind(&WHPSTcpSession::onNewClose, this, 0));   // 注册连接关闭回调

        // 还需要注册发送数据和超时回调
}

WHPSTcpSession::~WHPSTcpSession()
{
        cout << "~WHPSTcpSession..." << endl;
}

bool WHPSTcpSession::isValid()
{
        return _conn_sock.isValid();
}

const int& WHPSTcpSession::get() const
{
        return _conn_sock.get();
}

WHPSConnSocket& WHPSTcpSession::getConn()
{
        return _conn_sock;
}

void WHPSTcpSession::addToEventLoop()
{
        _loop.addEvent(&_event_chn);    // 所有请求都由主线程处理(后续要修改成多线程)
}

void WHPSTcpSession::delFromEventLoop()
{
        _loop.delEvent(&_event_chn);
}

void WHPSTcpSession::setCleanUpCallback(TcpSessionCB& cb)
{
        _cb_cleanup = cb;
}

void WHPSTcpSession::onNewRead(error_code error)
{
        cout << "++++++++++++++++++++++++++" << endl;
        while (true)
        {
                unsigned char buffer[1024];
                int nbyte = read(_conn_sock.get(), buffer, 1024); 

                if (nbyte > 0)
                {
                        cout << getHexString(buffer, nbyte) << endl;
                }
                else if (nbyte == 0)    // 对端关闭
                {
                        onNewClose(-1); // 关闭连接
                        break;
                }
                else
                {
                        cout << "nbyte: " << nbyte << endl;     // 异步时，当缓冲区无数据时，read会返回-1,即：读完了
                        break;
                }
        }
        cout << "++++++++++++++++++++++++++" << endl;
}

void WHPSTcpSession::onNewClose(error_code error)
{
        /* 关闭数据需要做两件事情
         *      （1）关闭socket
         *      （2）通知主socket对象和EventLoop对象删除该句柄资源
         */

        cout << "WHPSTcpSession::onNewClose" << endl;
        //sp_TcpSession& sp_tcp_session = std::make_shared<WHPSTcpSession>(*this);
//        _cb_cleanup(sp_tcp_session);  // 执行清理回调函数
        //sp_TcpSession sp_tcp_session = shared_from_this();
        this->delFromEventLoop();
        sp_TcpSession sp_tcp_session = shared_from_this();      // 返回this类的智能指针
        _cb_cleanup(sp_tcp_session);
        _conn_sock.close();
}
