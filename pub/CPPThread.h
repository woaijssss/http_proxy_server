#ifndef __CPP_THREAD_H__
#define __CPP_THREAD_H__

#include <thread>

#include "ImplThread.h"
#include "Task.h"

// typedef std::thread::id thread__id;

/* c++版本的线程
 * 实现了线程创建、销毁和启停动作的功能
 */
class CPPThread: public ImplThread
{
public:
        using task_t = task_func_t;
        using impl_task_func_t = __impl_task_func_t;
public:
        CPPThread(Task<task_t>& task);
        virtual ~CPPThread();

public:
        /* 启动线程 */
        virtual void start();
        virtual void start(impl_task_func_t callback);

        /* 停止线程 */
        virtual void stop();

        /* 等待线程结束，回收资源 */
        virtual void join();

        /* 获取当前线程的id号 */
        virtual const thread__id& getId() const;

public:
        /* 线程执行函数 */
        void workFunc();

        void excuteTask();

private:
        /* 线程进入执行函数后的一些初始化 */
        virtual void init();
private:
        std::thread m_thrd;     // 线程句柄
        thread__id m_tid;      // 线程id
        bool m_isStop;         // 线程退出标志

private:
        Task<task_t>& m_task;    // 任务队列(共享线程池中的队列)
};

#endif  // __CPP_THREAD_H__
