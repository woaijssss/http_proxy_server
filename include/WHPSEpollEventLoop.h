
#ifndef __WHPS_EPOLL_EVENT_LOOP_H__
#define __WHPS_EPOLL_EVENT_LOOP_H__

/* 事件循环类，核心功能为等待epoll事件的返回
 * 该任务由工作线程来执行
 * 若有事件返回，回调到外部callback中。
 */
#include "WHPSPoller.h"

class WHPSEpollEventLoop
{
public:
        WHPSEpollEventLoop();
        ~WHPSEpollEventLoop();

public:
        /* 添加事件 */
        void addEvent(event_chn* p_event);

        /* 移除事件 */
        void delEvent(event_chn* p_event);

        /* 修改事件 */
        void updateEvent(event_chn* p_event);

        /* 核心：事件循环任务 
         * 所有工作线程都应工作在此(包括主线程和子线程)
         *   - 主线程：用于接收、断开客户端连接；
         *   - 子线程：用于接收、发送客户端数据；
         */
        void loop();
private:
        WHPSPoller _poller;
};

#endif  // __WHPS_EPOLL_EVENT_LOOP_H__