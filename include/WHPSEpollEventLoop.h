
#ifndef __WHPS_EPOLL_EVENT_LOOP_H__
#define __WHPS_EPOLL_EVENT_LOOP_H__

/* 事件循环类，核心功能为等待epoll事件的返回
 * 该任务由工作线程来执行
 * 若有事件返回，回调到外部callback中。
 */
#include "WHPSPoller.h"
#include "vector.h"
#include "Task.h"

class WHPSEpollEventLoop
{
public:
        using task_t = std::function<void()>;   // 线程任务队列的任务类型(应与ImplThread中的任务类型相同)
public:
        WHPSEpollEventLoop(int maxevents = 1024, int timeout = 100);
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

        /* 子线程执行，循环由线程控制，函数内部只执行一次 */
        void loopOne();      

        /* 获取当前循环的任务队列 */
        Task<task_t>& getTask();

        /* 通过事件处理，添加一个异步任务 */
        void addTask(task_t func_cb);

public:         // 测试接口
        void stop();

private:
        WHPSPoller _poller;                     // 事件轮询对象

        /* 事件队列 
         * 外部线程，通过事件队列，来向epoll获取任务
         */
        Vector<event_chn*> _event_queue;

        bool _is_stop;

        Task<task_t> _task;     // 线程任务队列
};

#endif  // __WHPS_EPOLL_EVENT_LOOP_H__
