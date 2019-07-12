
#include <iostream>
#include <unistd.h>
using namespace std;

#include "WHPSTcpSession.h"
#include "util.h"

static void TestSend(WHPSTcpSession* sp_tcp_session)
{
        string msg;
        for (int i = 0; i < 300; i++)
        {
                //string msg((char*)p);
                msg += (char)0xaa;
        }
        sp_tcp_session->send(msg);
}

WHPSTcpSession::WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr)
        : std::enable_shared_from_this<WHPSTcpSession>()
          , _c_addr(c_addr)
          , _loop(loop)
          , _conn_sock(fd)
          , _base_events(EPOLLIN | EPOLLPRI)
{
        /* 每个客户端的socket要设置成非阻塞，否则在read或write会使线程阻塞，无法实现异步和线程复用 */
        _conn_sock.setNonblock();

        _event_chn.setFd(_conn_sock.get());
        // _event_chn.setEvents(EPOLLIN | EPOLLPRI | EPOLLOUT); // 不能设置EPOLLOUT，否则客户端连接，会频繁调用onNewWrite()
        _event_chn.setEvents(_base_events);        // 设置接收连接事件，epoll模式为边缘触发

        _event_chn.setReadCallback(std::bind(&WHPSTcpSession::onNewRead, this, 0));     // 注册数据接收回调函数
        _event_chn.setWriteCallback(std::bind(&WHPSTcpSession::onNewWrite, this, 0));   // 注册数据发送回调函数
        _event_chn.setCloseCallback(std::bind(&WHPSTcpSession::onNewClose, this, 0));   // 注册连接关闭回调函数
        _event_chn.setCloseCallback(std::bind(&WHPSTcpSession::onNewError, this, 0));   // 注册异常错误回调函数

        // 还需要注册发送数据和超时回调
}

WHPSTcpSession::~WHPSTcpSession()
{
        cout << "~WHPSTcpSession" << endl;
        this->delFromEventLoop();
        _conn_sock.close();
        // _conn_sock.close();
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

void WHPSTcpSession::send(const std::string& msg)
{
        _buffer_out = msg;
        int res = this->sendTcpMessage(_buffer_out);      // 需要判别发送的结果，从而决定要不要关闭连接
        _buffer_out.clear();

        if (res > 0)    // 正确发送数据
        {
                events_t events = _event_chn.getEvents();

                if (_buffer_out.size())     // 还有数据未发送(可能是tcp缓冲区占满导致)
                {
                        events = events | EPOLLOUT;         // 设置写监听
                }
                else        // 数据发送完毕
                {
                        events = _base_events;      // 数据发送完毕，无需继续监听写操作
                }
                
                _event_chn.setEvents(events);    
                _loop.updateEvent(&_event_chn);
        }
        else if (res == 0)    // 数据发送异常
        {
                onNewClose(-1); // 关闭连接
        }
        else
        {
                onNewError(-1); // 错误处理，释放资源
        }

        // cout << "-----------buff size: " << _buffer_out.size() << endl;
        // _buffer_out.clear();
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
                int w_nbytes = write(_conn_sock.get(), buffer_out.c_str(), buffer_out.size());
                bytes_transferred += w_nbytes;

                if (w_nbytes > 0)   // 该笔数据已发送
                {
                        buffer_out.erase(0, w_nbytes);     // 清除缓冲区中已发送的数据(不适用于重发的情况)

                        if (_buffer_out.size() == 0)
                        {
                                res = bytes_transferred;
                                break;
                        }

                }
                else if (w_nbytes == 0)
                {
                        cout << "w_nbytes----errno: " << w_nbytes << "----" << errno << endl;
                        if (errno == EAGAIN)    // 数据正确的发送完成，再次调用的返回结果
                        {
                                cout << "write errno == EAGAIN, 发送完成!---" << endl;
                                res = bytes_transferred;
                                // res = 0;
                        }

                        break;
                }
                else    // 写数据异常
                {
                        cout << "write error: w_nbytes----errno: " << w_nbytes << "----" << errno << endl;
                        if (errno == EAGAIN)    // tcp发送缓冲区满了，下次继续发送
                        {
                                cout << "write errno == EAGAIN, tcp发送缓冲区满了，下次继续发送!" << bytes_transferred << endl;
                                // res = bytes_transferred;

                                /* 这里统一返回1！
                                 *  若返回bytes_transferred，当写一次出错时，同时errno是EAGAIN，不应关闭
                                 *  但实际的bytes_transferred结果为-1，会导致异常关闭。
                                 */
                                res = 1;   
                        }
                        else if (errno == EPIPE)     // 客户端已经close，并发了RST，继续wirte会报EPIPE，返回0，表示close
                        {
                                cout << "EPIPE..." << endl;
                        }
                        else if (errno == EINTR)    // 中断，write()会返回-1，同时置errno为EINTR
                        {

                        }
                        else
                        {
                                cout << "unknow errno type..." << endl;
                        }

                        break; 
                }
        }

        return res;
}

#include <thread>

void WHPSTcpSession::onNewRead(error_code error)
{
        int res = this->readTcpMessage(_buffer_in);

        _buffer_in.clear();

        if (res > 0)
        {
                // 进行业务层的数据数据(tcp数据处理)
                // 异步回调方式，epoll线程跟业务线程分离
                cout << "thread_id: " << (unsigned int)std::hash<std::thread::id>()(std::this_thread::get_id()) 
                    << "----" << getHexString((unsigned char*)_buffer_in.c_str(), res) << endl;

#if 1           // 测试socket关闭后，再发送数据的错误处理
                TestSend(this);     
                // 测试发送接口修改成通过向任务队列抛任务的方式，由不同的线程异步执行
#endif
        }
        else if (res == 0)
        {
                onNewClose(-1); // 关闭连接
        }
        else
        {
                onNewError(-1); // 错误处理，释放资源
        }

        // _buffer_in.clear();
}

int WHPSTcpSession::readTcpMessage(std::string& buffer_in)
{
        int res = -1;
        int bytes_transferred = 0;

        while (true)
        {
                char buffer[1024];
                int r_nbyte = read(_conn_sock.get(), buffer, 1024);

                if (r_nbyte > 0)
                {
                        // cout << getHexString(buffer, r_nbyte) << endl;
                        _buffer_in.append(buffer, r_nbyte);
                        bytes_transferred += r_nbyte;
                }
                else if (r_nbyte == 0)    // 客户端关闭socket，FIN包
                {
                        cout << "---------======" << endl;
                        res = 0;
                        break;
                }
                else    // 读数据异常(-1)
                {
                        cout << "r_nbyte----errno: " << r_nbyte << "----" << errno << endl;     // 异步时，当缓冲区无数据时，read会返回-1,即：读完了

                        if (errno == EAGAIN)    // 在非阻塞模式下调用了阻塞操作，在该操作没有完成就返回这个错误，这个错误不会破坏socket的同步，下次循环接着recv就可以。
                        {
                                // pass
                                res = bytes_transferred;
                        }
                        else if (errno == EINTR)    // 中断，read()会返回-1，同时置errno为EINTR
                        {

                        }

#if 0   // send msg test(测试发送的异常情况)
                        TestSend(this);
#endif
                        break;
                }
        }

        return res;
}

void WHPSTcpSession::onNewWrite(error_code error)
{
        cout << "-----------------------WHPSTcpSession::onNewWrite: " << _buffer_out.size() << endl;
        int res = this->sendTcpMessage(_buffer_out);      // 需要判别发送的结果，从而决定要不要关闭连接
        _buffer_out.clear();

        if (res > 0)    // 正确发送数据
        {
                events_t events = _event_chn.getEvents();

                if (_buffer_out.size())     // 还有数据未发送(可能是tcp缓冲区占满导致)
                {
                        events = events | EPOLLOUT;         // 设置写监听
                }
                else        // 数据发送完毕
                {
                        events = _base_events;      // 数据发送完毕，无需继续监听写操作
                }
                
                _event_chn.setEvents(events);    
                _loop.updateEvent(&_event_chn);
        }
        else if (res == 0)    // 数据发送异常
        {
                onNewClose(-1); // 关闭连接
        }
        else
        {
                onNewError(-1); // 错误处理，释放资源
        }
}

void WHPSTcpSession::onNewClose(error_code error)
{
        /* 关闭数据需要做两件事情
         *      （1）处理剩余数据(接收_buffer_in/发送_buffer_out)————(暂不考虑，后续扩展)
         *      （2）关闭socket
         *      （3）通知主socket对象和EventLoop对象删除该句柄资源
         */
        if (_buffer_in.size() || _buffer_out.size())
        {
                cout << "WHPSTcpSession::simulator: " << _buffer_in.size() << "|" << _buffer_out.size() << endl;
                // 模拟全部数据处理完成
                _buffer_in.clear();
                _buffer_out.clear();
        }
        else
        {
                cout << "WHPSTcpSession::onNewClose: " << _buffer_in.size() << "|" << _buffer_out.size() << endl;
                //sp_TcpSession& sp_tcp_session = std::make_shared<WHPSTcpSession>(*this);
        //        _cb_cleanup(sp_tcp_session);  // 执行清理回调函数
                //sp_TcpSession sp_tcp_session = shared_from_this();
                // this->delFromEventLoop();
                //sp_TcpSession sp_tcp_session = shared_from_this();      // 返回this类的智能指针
                _cb_cleanup(shared_from_this());

                // _loop.addTask(std::bind(_cb_cleanup, shared_from_this()));
       }
}

void WHPSTcpSession::onNewError(error_code error)
{
        /* 关闭数据需要做两件事情
         *      （1）关闭socket
         *      （2）通知主socket对象和EventLoop对象删除该句柄资源
         */

        cout << "WHPSTcpSession::onNewError" << endl;
        //sp_TcpSession& sp_tcp_session = std::make_shared<WHPSTcpSession>(*this);
//        _cb_cleanup(sp_tcp_session);  // 执行清理回调函数
        //sp_TcpSession sp_tcp_session = shared_from_this();
        // this->delFromEventLoop();
        //sp_TcpSession sp_tcp_session = shared_from_this();      // 返回this类的智能指针
        _cb_cleanup(shared_from_this());
        // _conn_sock.close();
}
