
#include <string.h>

#include "WHPSPoller.h"

WHPSPoller::WHPSPoller(int maxevents, int timeout)
        : Poller(maxevents, timeout)
{

}
       
WHPSPoller::~WHPSPoller()
{

}

void WHPSPoller::addEvent(event_chn* p_event)
{
        this->eventOperation(ADD, p_event);
}

void WHPSPoller::delEvent(event_chn* p_event)
{
        this->eventOperation(DEL, p_event);
}

void WHPSPoller::updateEvent(event_chn* p_event)
{
        this->eventOperation(UPDATE, p_event);
}

void WHPSPoller::eventOperation(OpsType type, event_chn* p_event)
{
        int fd = p_event->getFd();
        SEplEvent evt;
        memset(&evt, 0, sizeof(evt));
        evt.events = p_event->getEvents();
        evt.data.fd = fd;
        evt.data.ptr = p_event;
        switch (type)
        {
        case ADD:
        {
                this->addToEventList(fd, p_event);
                this->__addEvent(fd, evt);
                break;
        }
        case DEL:
        {
                this->delFromEventList(fd);
                this->__delEvent(fd, evt);
                break;
        }
        case UPDATE:
        {
                this->__updateEvent(fd, evt);
                break;
        }
        }
}

int WHPSPoller::epollWait()
{
        return this->__poll(_events);
}

#include <iostream>
using namespace std;
int WHPSPoller::poll(Vector<event_chn*>& event_queue)
{
        int n_fds = this->epollWait();

        if (n_fds < 0)
        {
                cerr << "epoll wait error, err: " << strerror(errno) << endl;
                return -1;
        }

        this->onEvent(event_queue, n_fds);

        return 0;
}

#if 0
        typedef union epoll_data
        {
                void *ptr;
                int fd;
                uint32_t u32;
                uint64_t u64;
        } epoll_data_t;

        struct epoll_event
        {
                uint32_t events;      /* Epoll events */
                epoll_data_t data;    /* User data variable */
        } __EPOLL_PACKED;
#endif
void WHPSPoller::onEvent(Vector<event_chn*>& event_queue, const int& n_fds)
{
        for (int i = 0; i < n_fds; i++)
        {
                int events = _events[i].events;                         // 获取被触发的事件类型
                event_chn* p_event = (event_chn*)_events[i].data.ptr;   // 获取被触发的事件指针
                const int& fd = p_event->getFd();                       // 获取事件对应的句柄
                Map<int, event_chn*>::iterator it = _event_list.find(fd);
                if (it != _event_list.end())    // 在事件表中找到了对应的句柄
                {
                        p_event->setEvents(events);     // 设置句柄本次触发的事件类型
                        event_queue.push_back(p_event); // 向队列添加事件
                }
        }
}



void WHPSPoller::addToEventList(int fd, event_chn* p_event)
{
        _event_list.insert(fd, p_event);
}

void WHPSPoller::delFromEventList(int fd)
{
        _event_list.erase(fd);
}

/*
void WHPSPoller::updateEventList(int fd)
{
        // nothing to do
}
*/
