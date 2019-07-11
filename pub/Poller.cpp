
#include "Poller.h"
#include "util.h"

Poller::Poller(int maxevents, int timeout)
        : __poll_fd(INVALID_EPOLLFD), __maxevents(maxevents), __timeout(timeout)
{
        __poll_fd = epoll_create(1024);

        if (__poll_fd == INVALID_EPOLLFD)
        {
                exitService(INVALID_EPOLLFD);
        }
}

Poller::~Poller()
{
        
}

#include <iostream>
using namespace std;
int Poller::__poll(SEplEvent* events)
{
        // cout << ">>>>>>>>>>>epoll_wait timeout: " << __timeout << endl;
        return epoll_wait(__poll_fd, events, __maxevents, __timeout);
}

int Poller::__addEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(__poll_fd, EPOLL_CTL_ADD, fd, &ev);
}

int Poller::__delEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(__poll_fd, EPOLL_CTL_DEL, fd, &ev);
}

int Poller::__updateEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(__poll_fd, EPOLL_CTL_MOD, fd, &ev);
}

void Poller::__setTimeout(int timeout)
{
        __timeout = timeout;
}
