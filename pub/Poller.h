
#ifndef __POLLER_H__
#define __POLLER_H__

#define INVALID_EPOLLFD    -1

/*
 * 基础epoll类，该类实现对linux系统库中，epoll基本功能的封装。
 */
class Poller
{
public:
        Poller();
        ~Poller();

public:


private:
        int _poll_fd;          // epoll实例句柄
};

#endif  __EPOLLER_H__