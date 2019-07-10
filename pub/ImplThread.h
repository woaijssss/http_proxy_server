
#ifndef __IMPL_THREAD_H__
#define __IMPL_THREAD_H__

#include <functional>

typedef unsigned int thread__id;
typedef std::function<void()> __impl_task_func_t;

/* 线程(虚)接口类
 * 子类可以是c语言版的线程，也可以是c++版本的线程
 */
class ImplThread
{
public:
        virtual inline ~ImplThread()
        {

        }

        /* 启动线程
         * 一般进行对线程执行状态标志位置位、初始化参数等操作
         */
        virtual void start() = 0;
        virtual void start(__impl_task_func_t __callback) = 0;  // 可以设置线程的执行函数

        /* 停止线程
         * 一般进行对线程执行状态标志位初始化操作
         */
        virtual void stop() = 0;

        /* 等待线程结束，回收资源 */
        virtual void join() = 0;

        /* 获取当前线程的id号 */
        virtual const thread__id& getId() const = 0;

public:
        /* 线程执行函数(c和c++对于执行函数的参数和返回值都不同，这里不做统一) */
        //virtual void workFunc() = 0;

        /* 线程进入执行函数后的一些初始化 */
        virtual void init() = 0;
};

#endif  // __IMPL_THREAD_H__