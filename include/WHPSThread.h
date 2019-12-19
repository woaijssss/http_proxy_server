
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
        CPPThread m_thrd;
        bool m_isStop;
        WHPSEpollEventLoop m_loop;
};

class WHPSWorkerThread
{
public:
        using task_func_t = CPPThread::impl_task_func_t;
        using task_t = CPPThread::task_t;
public:
        WHPSWorkerThread(Task<task_t>& task);
        ~WHPSWorkerThread();

public:
        void start();

        void stop();

private:
        void workFunc();

        void excuteTask();
private:
        CPPThread m_thrd;
        bool m_isStop;
};

#endif  // __WHPS_THREAD_H__
