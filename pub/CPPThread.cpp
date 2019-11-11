#include <iostream>
#include <unistd.h>
using namespace std;

#include "CPPThread.h"
#include "WHPSLog.h"

CPPThread::CPPThread(Task<task_t>& task)
        : m_tid(0),
          m_isStop(false),
          m_task(task)
{

}

CPPThread::~CPPThread()
{
        // this->join();
}

void CPPThread::start()
{
        m_thrd = std::thread(std::bind(&CPPThread::workFunc, this));
        this->init();
}

void CPPThread::start(impl_task_func_t callback)
{
        m_thrd = std::thread(callback);
        this->init();
}

void CPPThread::stop()
{
        m_isStop = true;
}

void CPPThread::join()
{
        if (!m_isStop)
        {
                this->stop();
                m_thrd.join();
        }
}

const thread__id& CPPThread::getId() const
{
        return m_tid;
}

void CPPThread::init()
{
        /* 基类统一tid的数据类型，因此使用std::hash，将c++版的线程id，转换成可与c版本的通用类型：unsigned int */
        m_tid = std::hash<std::thread::id>()(m_thrd.get_id());
        m_isStop = false;
}

void CPPThread::excuteTask()
{
        /* 执行所有任务 */

//        for (size_t i = 0; i < m_task.size(); i++)
//        {
        try
        {
                task_t task = m_task.get();

                if (task)       // 有可能存在取空的情况，因此加上判断，防止执行空函数
                {
                        task();
                }
        } catch (exception& e)
        {
                WHPSLogInfo("CPPThread::excuteTask %s", e.what());
        }
//        }
}

#if 1
void CPPThread::workFunc()
{
        /* 每个线程在执行函数中，仅仅关心队列中的任务，不需要关心任务的来源 */

        while (!m_isStop)
        {
                usleep(10);     // 延迟后面可去掉，因为一次http业务处理远比10us长
                task_t task = m_task.get();
                // task();
                if (task)       // 有可能存在取空的情况，因此加上判断，防止执行空函数
                {
                        task();
                }
        }
}
#endif
