
#ifndef __WHPS_THREAD_POOL_H__
#define __WHPS_THREAD_POOL_H__

#include "ThreadPool.h"

class WHPSThreadPool
{
public:
        WHPSThreadPool(int nthreads, task_func_t callback);
        ~WHPSThreadPool();

public:
        /* 启动线程池任务 */
        void start();

private:
        ThreadPool _tp;
};

#endif  // __WHPS_THREAD_POOL_H__