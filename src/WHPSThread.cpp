#include "WHPSThread.h"

#include <iostream>
using namespace std;
WHPSThread::WHPSThread(Task<task_t>& task)
        : m_thrd(m_loop.getTask()),
          m_isStop(false)
{

}

WHPSThread::~WHPSThread()
{
        this->stop();
        m_thrd.join();
}

void WHPSThread::start()
{
        task_func_t callback = std::bind(&WHPSThread::workFunc, this);
        m_thrd.start(callback);
}

void WHPSThread::stop()
{
        m_isStop = true;
        m_loop.stop();
}

WHPSEpollEventLoop& WHPSThread::getLoop()
{
        return m_loop;
}

void WHPSThread::workFunc()
{
        while (!m_isStop)
        {
                /* 调用执行一次epoll任务的接口，因为线程还要处理Task queue中的内容 */
                m_loop.loopOne();

                /* 处理Task queue中的内容，每次只取一个 */
//                this->excuteTask();
        }
}

void WHPSThread::excuteTask()
{
        m_thrd.excuteTask();
}
