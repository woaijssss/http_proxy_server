
#ifndef __IMPL_EVENT_HANDLER_H__
#define __IMPL_EVENT_HANDLER_H__

#include <functional>

#define __stdcall
/*
 * 事件回调抽象接口，包含：
 *      类型声明、回调接口声明、epoll事件类型、提供当前子类对应的句柄描述符等。
 * 任何回调事件的触发都是通过__fd完成；
 * 任何与__fd相关的回调，都是用过epoll events来触发。
 */
class ImplEventHandler
{
public: // 类型定义
        /* 预定义回调函数类型 */
        using __callback_t = std::function<void()>;
        using __epoll_events_t = unsigned int;

public:
        virtual ~ImplEventHandler()
        {

        }

protected:
        /* 设置当前处理的句柄描述符 */
        void __setFd(int fd)
        {
                __fd = fd;
        }

        /* 获取已设置的句柄描述符 */
        const int& __getFd()
        {
                return __fd;
        }

        /* 设置epoll事件类型宏 */
        void __setEvents(__epoll_events_t events)
        {
                __epoll_events = events;
        }

        /* 获取epoll事件类型宏 */
        const __epoll_events_t& __getEvents() const
        {
                return __epoll_events;
        }

        /* 设置回调函数接口 */
        virtual void __stdcall __setCallback(__callback_t& __cb_s, __callback_t __cb_d) = 0;

        /* 执行回调函数接口 */
        virtual void __stdcall __exCallback() = 0;

private:
        /* 该父类的派生类所拥有的句柄描述符。
         * 任何的事件处理都是基于句柄描述符，该描述符的值应与外部统一。
         */
        int __fd;

        /* epoll事件类型，其值类型为 enum EPOLL_EVENTS */
        unsigned int __epoll_events;
};

#endif  // __IMPL_EVENT_HANDLER_H__
