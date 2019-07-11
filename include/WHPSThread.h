
#ifndef __WHPS_THREAD_H__
#define __WHPS_THREAD_H__

#include "WHPSEpollEventLoop.h"
#include "CPPThread.h"

class WHPSThread
{
public:
        using task_func_t = CPPThread::impl_task_func_t;
        using task_t = CPPThread::task_t;
public:
        WHPSThread(Task<task_t>& task);
        ~WHPSThread();

public:
        void start();

        void stop();

        WHPSEpollEventLoop& getLoop();

private:
        void workFunc();

        void excuteTask();
private:
        CPPThread _thrd;
        bool _is_stop;
        WHPSEpollEventLoop _loop;
};

#endif  // __WHPS_THREAD_H__