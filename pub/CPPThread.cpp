
#include <iostream>
#include <unistd.h>
using namespace std;

#include "CPPThread.h"

CPPThread::CPPThread()
        : __is_stop(false)
{

}

CPPThread::~CPPThread()
{
        this->join();
}

void CPPThread::start()
{
        __thrd = std::thread(std::bind(&CPPThread::workFunc, this));
}

void CPPThread::stop()
{
        __is_stop = true;
}

void CPPThread::join()
{
        cout << "this thread: " << __tid << " join" << endl;
        this->stop();
        __thrd.join();
}

const thread__id& CPPThread::getId() const
{
        return __tid;
}

void CPPThread::init()
{
        /* 基类统一tid的数据类型，因此使用std::hash，将c++版的线程id，转换成可与c版本的通用类型：unsigned int */
        __tid = std::hash<std::thread::id>()(std::this_thread::get_id());
        __is_stop = false;
}

void CPPThread::workFunc()
{
        this->init();

        while (!__is_stop)
        {
                sleep(1);
                cout << __tid << "=========" << endl;
        }
}