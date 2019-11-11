#include "Poller.h"
#include "util.h"

Poller::Poller(int maxevents, int timeout)
        : m_pollFd(INVALID_EPOLLFD),
          m_maxEvents(maxevents),
          m_timeout(timeout)
{
        m_pollFd = epoll_create(1024);

        if (m_pollFd == INVALID_EPOLLFD)
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
        return epoll_wait(m_pollFd, events, m_maxEvents, m_timeout);
}

int Poller::__addEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(m_pollFd, EPOLL_CTL_ADD, fd, &ev);
}

int Poller::__delEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(m_pollFd, EPOLL_CTL_DEL, fd, &ev);
}

int Poller::__updateEvent(const int& fd, SEplEvent& ev)
{
        return epoll_ctl(m_pollFd, EPOLL_CTL_MOD, fd, &ev);
}

void Poller::__setTimeout(int timeout)
{
        m_timeout = timeout;
}
