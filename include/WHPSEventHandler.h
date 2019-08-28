
#ifndef __WHPS_EVENT_HANDLER_H__
#define __WHPS_EVENT_HANDLER_H__

#include <mutex>

#include "ImplEventHandler.h"
#include "WHPSTcpSocket.h"

class WHPSEventHandler;

typedef ImplEventHandler::__callback_t CbFunc;
typedef ImplEventHandler::__epoll_events_t events_t;
typedef WHPSEventHandler event_chn;

enum EventType
{

};

/* 事件注册、回调类：
 *  （1）设置指定的fd
 *  （2）设置指定的epoll触发类型
 *  （3）设置指定的回调函数
 */
class WHPSEventHandler : public ImplEventHandler
{
public:
        WHPSEventHandler();
        virtual ~WHPSEventHandler();

public:
        void stop();

        /* 设置当前处理的句柄描述符 */
        void setFd(const int& fd);

        /* 获取已设置的句柄描述符 */
        const int& getFd();

        /* 设置epoll事件类型宏 */
        void setEvents(events_t events);

        /* 获取epoll事件类型宏 */
        const events_t& getEvents() const;

        /* 设置读事件回调函数 */
        void setReadCallback(CbFunc cb);

        /* 设置写事件回调函数 */
        void setWriteCallback(CbFunc cb);

        /* 设置错误事件回调函数 */
        void setErrorCallback(CbFunc cb);

        /* 设置关闭句柄事件回调函数 */
        void setCloseCallback(CbFunc cb);


        /* 执行事件回调函数 */
        virtual void exCallback();

private:
        /* 设置事件回调函数：
         *      任何设置事件回调的过程，都通过此接口实现；
         * 禁止外部回调
         */
        virtual void setCallback(CbFunc& cb_s, CbFunc cb_d);

        virtual void __stdcall __setCallback(__callback_t& __cb_s, __callback_t __cb_d);

        /* 执行回调函数接口 */
        virtual void __stdcall __exCallback();

        void onCall(CbFunc cb);

private:
        CbFunc _cb_read;      // 读回调
        CbFunc _cb_write;     // 写回调
        CbFunc _cb_error;     // 错误回调
        CbFunc _cb_close;     // 关闭回调

        bool _is_stop /*= false*/;
        std::mutex _mutex;
};

#endif  // __WHPS_EVENT_HANDLER_H__
