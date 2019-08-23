
#include <iostream>
#include <unistd.h>
using namespace std;

#include "CPPThread.h"

CPPThread::CPPThread(Task<task_t>& task)
        : __is_stop(false)
        , _task(task)
{

}

CPPThread::~CPPThread()
{
        // this->join();
}

void CPPThread::start()
{
        __thrd = std::thread(std::bind(&CPPThread::workFunc, this));
        this->init();
}

void CPPThread::start(impl_task_func_t callback)
{
        __thrd = std::thread(callback);
        this->init();
}

void CPPThread::stop()
{
        __is_stop = true;
}

void CPPThread::join()
{
        if (!__is_stop)
        {
                this->stop();
                __thrd.join();
        }
}

const thread__id& CPPThread::getId() const
{
        return __tid;
}

void CPPThread::init()
{
        /* 基类统一tid的数据类型，因此使用std::hash，将c++版的线程id，转换成可与c版本的通用类型：unsigned int */
        __tid = std::hash<std::thread::id>()(__thrd.get_id());
        __is_stop = false;
}

void CPPThread::excuteTask()
{
        /* 执行所有任务 */

        for (size_t i = 0; i < _task.size(); i++)
        {
                task_t task = _task.get();

                if (task)       // 有可能存在取空的情况，因此加上判断，防止执行空函数
                {
			cout << "CPPThread::excuteTask" << endl;
                        task();
                }
        }
}

#if 1
void CPPThread::workFunc()
{
        /* 每个线程在执行函数中，仅仅关心队列中的任务，不需要关心任务的来源 */

        while (!__is_stop)
        {
                usleep(10);     // 延迟后面可去掉，因为一次http业务处理远比10us长
                task_t task = _task.get();
                // task();
                if (task)       // 有可能存在取空的情况，因此加上判断，防止执行空函数
                {
                        task();
                }
        }
}
#endif
