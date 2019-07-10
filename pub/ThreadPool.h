
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
        explicit ThreadPool(int size);
        explicit ThreadPool(int size, task_func_t callback);    // 可以设置线程的执行函数
        ~ThreadPool();

public:
        /* 所有线程开始执行任务 */
        void start();

        /* 测试接口：主线程向队列中添加任务，由所有子线程来执行 */
        void testTask(task_func_t task)
        {
                __task.addTask(task);
        }

private:
        /* 创建一定数量的线程 */
        void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        void stop();

private:
        int __size;     // 线程数量
        task_func_t     __callback;             // 记录线程的执行函数
        std::vector<ImplThread*> __v_th;        // 线程列表

        Task<task_func_t> __task;          // 线程任务(当前是所有线程共享一个任务队列)
};

#endif  // __THREAD_POOL_H__