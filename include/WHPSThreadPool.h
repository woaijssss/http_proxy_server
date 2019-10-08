
#ifndef __WHPS_THREAD_POOL_H__
#define __WHPS_THREAD_POOL_H__

#include "WHPSEpollEventLoop.h"
#include "ImplThreadPool.h"
#include "WHPSThread.h"

/* I/O线程池，主要负责：
 * （1）收发tcp数据；
 * （2）处理业务功能
 */
class WHPSThreadPool : public ImplThreadPool
{
public:
        /* 线程池入口，可设置工作线程数量 */
        explicit WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop);
        explicit WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop, task_func_t callback);    // 可以设置线程的执行函数
        
        virtual ~WHPSThreadPool();

public:
        WHPSEpollEventLoop& getOneLoop();

        /* 所有线程开始执行任务 */
        void start();

private:
        /* 创建一定数量的线程 */
        void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        void stop();

private:
        WHPSEpollEventLoop& _main_loop;         // 主循环(单线程时使用)
        int _index;
        int _size;     // 线程数量
        task_func_t     _callback;             // 记录线程的执行函数
        std::vector<WHPSThread*> _v_th;        // 线程列表

        Task<task_func_t> _task;          // 线程任务(当前是所有线程共享一个任务队列)
};

/* worker线程池，主要负责：
 * （1）连接 session 资源的释放
 */
class WHPSWorkerThreadPool : public ImplThreadPool
{
public:
        /* 线程池入口，可设置工作线程数量 */
        explicit WHPSWorkerThreadPool(int size);
        explicit WHPSWorkerThreadPool(int size, task_func_t callback);    // 可以设置线程的执行函数

        virtual ~WHPSWorkerThreadPool();

public:
        /* 所有线程开始执行任务 */
        void start();

        Task<task_func_t> _task;          // 线程任务(当前是所有线程共享一个任务队列)
private:
        /* 创建一定数量的线程 */
        void createThreads();

        /* 所有线程停止执行任务，释放资源并退出 */
        void stop();

private:
        int _index;
        int _size;     // 线程数量
        task_func_t     _callback;             // 记录线程的执行函数
        std::vector<WHPSWorkerThread*> _v_th;        // 线程列表
};

#endif  // __WHPS_THREAD_POOL_H__
