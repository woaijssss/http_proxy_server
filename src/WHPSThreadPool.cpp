
#include "WHPSThreadPool.h"

WHPSThreadPool::WHPSThreadPool(int nthreads, task_func_t callback)
        : _tp(nthreads, callback)
{

}

WHPSThreadPool::~WHPSThreadPool()
{

}

void WHPSThreadPool::start()
{
        _tp.start();
}