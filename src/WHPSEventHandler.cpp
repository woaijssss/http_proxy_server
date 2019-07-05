#include <sys/epoll.h>

#include "WHPSEventHandler.h"

WHPSEventHandler::WHPSEventHandler()
{

}

WHPSEventHandler::~WHPSEventHandler()
{

}

void WHPSEventHandler::setFd(const int& fd)
{
        __setFd(fd);
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
        this->setCallback(_cb_read, cb);
}

void WHPSEventHandler::setWriteCallback(CbFunc cb)
{
        this->setCallback(_cb_write, cb);
}

void WHPSEventHandler::setErrorCallback(CbFunc cb)
{
        this->setCallback(_cb_error, cb);
}

void WHPSEventHandler::setCloseCallback(CbFunc cb)
{
        this->setCallback(_cb_close, cb);
}

void WHPSEventHandler::setCallback(CbFunc& cb_s, CbFunc cb_d)
{
        this->__setCallback(cb_s, cb_d);
}

void WHPSEventHandler::exCallback()
{
        this->__exCallback();
}

void __stdcall WHPSEventHandler::__setCallback(__callback_t& __cb_s, __callback_t __cb_d)
{
        __cb_s = __cb_d;
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
                std::cout << "epoll type: EPOLLRDHUP" << std::endl;
                _cb_close();
        }
        else if (events & (EPOLLIN | EPOLLPRI))  //读事件，对端有数据或者正常关闭
        {
                std::cout << "epoll type: EPOLLIN" << std::endl;
                _cb_read();
        }
        else if (events & EPOLLOUT)  //写事件
        {
                std::cout << "epoll type: EPOLLOUT" << std::endl;
                _cb_write();
        }
        else
        {
                std::cout << "epoll type: error" << std::endl;
                _cb_error();  //连接错误
        }
}
