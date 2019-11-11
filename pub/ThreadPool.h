#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>

#include "ImplThreadPool.h"
#include "CPPThread.h"

#include "Task.h"

/* 线程池类
 * 可创建指定数量的线程，启动、销毁等线程的管理
 */
class ThreadPool: public ImplThreadPool
{
public:
        /* 线程池入口，可设置工作线程数量 */
        explicit ThreadPool(int size);
        explicit ThreadPool(int size, task_func_t callback);    // 可以设置线程的执行函数

        virtual ~ThreadPool();

public:
        /* 所有线程开始执行任务 */
        virtual void start();

        /* 测试接口：主线程向队列中添加任务，由所有子线程来执行 */
        void testTask(task_func_t task)
        {
                m_task.addTask(task);
        }

private:
        /* 创建一定数量的线程 */
        virtual void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        virtual void stop();

private:
        int m_size;     // 线程数量
        task_func_t m_callback;             // 记录线程的执行函数
        std::vector<ImplThread*> m_vTh;        // 线程列表

        Task<task_func_t> m_task;          // 线程任务(当前是所有线程共享一个任务队列)
};

#endif  // __THREAD_POOL_H__
