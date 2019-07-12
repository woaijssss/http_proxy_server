
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
        // cout << "clean up the WHPSThread id: " << _thrd.getId() << endl;
        this->stop();
        _thrd.join();
}

void WHPSThread::start()
{
        task_func_t callback = std::bind(&WHPSThread::workFunc, this);
        _thrd.start(callback);
        // cout << "this thread: " << _thrd.getId() << endl;
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
                this->excuteTask();
        }
}

void WHPSThread::excuteTask()
{
        _thrd.excuteTask();
}
