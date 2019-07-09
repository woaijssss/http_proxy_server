
#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>

#include "CPPThread.h"

#include "Task.h"

/* 线程池类
 * 可创建指定数量的线程，启动、销毁等线程的管理
 */
class ThreadPool
{
public:
        /* 线程池入口，可设置工作线程数量 */
        ThreadPool(int size);
        ~ThreadPool();

public:
        /* 所有线程开始执行任务 */
        void start();

private:
        /* 创建一定数量的线程 */
        void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        void stop();

private:
        int __size;     // 线程数量
        std::vector<ImplThread*> __v_th;     // 线程列表

        Task<std::function<void>()> _task;
};

#endif  // __THREAD_POOL_H__