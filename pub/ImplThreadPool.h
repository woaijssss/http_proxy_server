#ifndef __IMPL_THREAD_POLL_H__
#define __IMPL_THREAD_POLL_H__

/* 线程池接口类，具体线程吃的实现，继承此可有多种方法
 */
class ImplThreadPool
{
public:
        virtual ~ImplThreadPool()
        {
        }
        /* 所有线程开始执行任务 */
        virtual void start() = 0;

private:
        /* 创建一定数量的线程 */
        virtual void createThreads() = 0;

        /* 所有线程停止执行任务，释放资源并退出 */
        virtual void stop() = 0;
};

#endif  // __IMPL_THREAD_POLL_H__
