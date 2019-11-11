#ifndef __WHPS_THREAD_POOL_H__
#define __WHPS_THREAD_POOL_H__

#include "WHPSEpollEventLoop.h"
#include "ImplThreadPool.h"
#include "WHPSThread.h"

/* - I/O线程池，主要负责：
 *      （1）收发tcp数据；
 *      （2）处理业务功能
 * - worker线程池，主要负责：
 *      （1）连接 session 资源的释放
 */
class WHPSThreadPool: public ImplThreadPool
{
public:
        /* 线程池入口，可设置工作线程数量 */
        explicit WHPSThreadPool(int size, WHPSEpollEventLoop* main_loop = nullptr);
        explicit WHPSThreadPool(int size, task_func_t callback, WHPSEpollEventLoop* main_loop = nullptr); // 可以设置线程的执行函数

        virtual ~WHPSThreadPool();

public:
        WHPSEpollEventLoop& getOneLoop();

        /* 所有线程开始执行任务 */
        void start();

        void addTask(task_func_t task);

private:
        /* 创建一定数量的线程 */
        void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        void stop();

private:
        WHPSEpollEventLoop* m_mainLoop;         // 主循环(单线程时使用)
        int m_index;
        int m_size;                              // 线程数量
        task_func_t m_callBack;              // 记录线程的执行函数
        std::vector<WHPSThread*> m_vTh;         // 线程列表

        Task<task_func_t> m_task;                // 线程任务(当前是所有线程共享一个任务队列)
};

#endif  // __WHPS_THREAD_POOL_H__
