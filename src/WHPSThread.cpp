
#include "WHPSThread.h"

#include <iostream>
using namespace std;
WHPSThread::WHPSThread(Task<task_t>& task)
        : _thrd(_loop.getTask())
        , _is_stop(false)
{
        
}

WHPSThread::~WHPSThread()
{
        this->stop();
        _thrd.join();
}

void WHPSThread::start()
{
        task_func_t callback = std::bind(&WHPSThread::workFunc, this);
        _thrd.start(callback);
}

void WHPSThread::stop()
{
        _is_stop = true;
        _loop.stop();
}

WHPSEpollEventLoop& WHPSThread::getLoop()
{
        return _loop;
}

void WHPSThread::workFunc()
{
        while (!_is_stop)
        {
                /* 调用执行一次epoll任务的接口，因为线程还要处理Task queue中的内容 */
                _loop.loopOne();

                /* 处理Task queue中的内容，每次只取一个 */
//                this->excuteTask();
        }
}

void WHPSThread::excuteTask()
{
        _thrd.excuteTask();
}




WHPSWorkerThread::WHPSWorkerThread(Task<task_t>& task)
        : _thrd(task)
        , _is_stop(false)
{

}

WHPSWorkerThread::~WHPSWorkerThread()
{
        this->stop();
        _thrd.join();
}

void WHPSWorkerThread::start()
{
        task_func_t callback = std::bind(&WHPSWorkerThread::workFunc, this);
        _thrd.start(callback);
}

void WHPSWorkerThread::stop()
{
        _is_stop = true;
}

void WHPSWorkerThread::workFunc()
{
        while (!_is_stop)
        {
                /* 处理Task queue中的内容，每次只取一个 */
                this->excuteTask();
        }
}

void WHPSWorkerThread::excuteTask()
{
        _thrd.excuteTask();
}
