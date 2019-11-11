#include <sys/epoll.h>

#include "WHPSEventHandler.h"
#include "WHPSEpollEventLoop.h"

WHPSEventHandler::WHPSEventHandler(WHPSEpollEventLoop* loop)
        : m_loop(loop),
          m_isStop(false),
          m_cbRead(nullptr),
          m_cbWrite(nullptr),
          m_cbClose(nullptr),
          m_cbError(nullptr)
{

}

WHPSEventHandler::~WHPSEventHandler()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cbRead = nullptr;
        m_cbWrite = nullptr;
        m_cbError = nullptr;
        m_cbClose = nullptr;
}

void WHPSEventHandler::setFd(const int& fd)
{
        __setFd(fd);
}

void WHPSEventHandler::stop()
{
        m_isStop = true;
        m_loop->deleteOneChannel(this);
}

const int& WHPSEventHandler::getFd()
{
        return __getFd();
}

void WHPSEventHandler::setEvents(events_t events)
{
        __setEvents(events);
}

const events_t& WHPSEventHandler::getEvents() const
{
        return __getEvents();
}

void WHPSEventHandler::setReadCallback(CbFunc cb)
{
        this->setCallback(m_cbRead, cb);
//        m_cbRead = cb;
}

void WHPSEventHandler::setWriteCallback(CbFunc cb)
{
        this->setCallback(m_cbWrite, cb);
//        m_cbWrite = cb;
}

void WHPSEventHandler::setErrorCallback(CbFunc cb)
{
        this->setCallback(m_cbError, cb);
//        m_cbError = cb;
}

void WHPSEventHandler::setCloseCallback(CbFunc cb)
{
        this->setCallback(m_cbClose, cb);
//        m_cbClose = cb;
}

void WHPSEventHandler::setCallback(CbFunc& cb_s, CbFunc cb_d)
{
        this->__setCallback(cb_s, cb_d);
}

void WHPSEventHandler::exCallback()
{
        if (m_isStop)
        {
                return;
        }

        this->__exCallback();
}

void __stdcall WHPSEventHandler::__setCallback(__callback_t& __cb_s, __callback_t   __cb_d) {
std::lock_guard<std::mutex> lock(m_mutex);
__cb_s = __cb_d;
}

void WHPSEventHandler::onCall(CbFunc& cb)
{
        std::lock_guard<std::mutex> lock(m_mutex);
        if (cb)
        {
                cb();
        }
}

#include <iostream>
/* 执行回调函数接口 */
void __stdcall WHPSEventHandler::__exCallback()
{
        events_t events = this->getEvents();
        /* 在使用2.6.17之后版本内核的服务器系统中，对端连接断开触发的epoll事件会包含EPOLLIN | EPOLLRDHUP，即0x2001。
         */
        if (events & EPOLLRDHUP)         // 对端异常关闭事件
        {
//                m_cbClose();
                this->onCall(m_cbClose);
        } else if (events & (EPOLLIN | EPOLLPRI))  //读事件，对端有数据或者正常关闭
        {
//                m_cbRead();
                this->onCall(m_cbRead);
        } else if (events & EPOLLOUT)  //写事件
        {
//                m_cbWrite();
                this->onCall(m_cbWrite);
        } else    // 目前连接错误还没有测试过(未出现)
        {
//                m_cbError();  //连接错误
                this->onCall(m_cbError);
        }
}
