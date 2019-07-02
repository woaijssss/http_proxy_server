
#ifndef __WHPS_EVENT_HANDLER_H__
#define __WHPS_EVENT_HANDLER_H__

#include "ImplEventHandler.h"

typedef WHPSEventHandler::__callback_t CbFunc;
typedef WHPSEventHandler::__epoll_events_t events_t;

enum EventType
{

};

class WHPSEventHandler : public ImplEventHandler
{
public:
        WHPSEventHandler();
        ~WHPSEventHandler();

public:
        void setEvents(events_t events);
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
         */
        virtual void setCallback(CbFunc& cb_s, CbFunc, cb_d);

private:
        Callback _cb_read;      // 读回调
        Callback _cb_write;     // 写回调
        Callback _cb_error;     // 错误回调
        Callback _cb_close;     // 关闭回调
}

#endif  // __WHPS_EVENT_HANDLER_H__