
#ifndef __POLLER_H__
#define __POLLER_H__

#include <sys/epoll.h>

#define INVALID_EPOLLFD    -1
typedef struct epoll_event SEplEvent;

/*
 * 基础epoll类，该类实现对linux系统库中，epoll基本功能的封装。
 * 该类仅提供基本接口，不负责维护相关的数据结构。
 */
class Poller
{
public:
        Poller();
        ~Poller();

public:
        int __poll(SEplEvent* events);

        /* 添加事件，EPOLL_CTL_ADD */
        int __addEvent(const int& fd, SEplEvent& ev);

        /* 移除事件，EPOLL_CTL_DEL */
        int __delEvent(const int& fd, SEplEvent& ev);

        /* 修改事件，EPOLL_CTL_MOD */
        int __updateEvent(const int& fd, SEplEvent& ev);

private:
        int __poll_fd;          // epoll实例句柄
        int __maxevents;
        int __timeout;
};

#endif  // __EPOLLER_H__