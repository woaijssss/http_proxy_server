
#ifndef __WHPS_POLLER_H__
#define __WHPS_POLLER_H__

#include "WHPSEventHandler.h"
#include "Poller.h"

#include "map.h"
#include "vector.h"

#define MAX_EVENTS      1024            // 最大触发的事件数

/* WHPS事件循环基础类，用于启动事件和对事件的增、删、改、查。
 * WHPSPoller中，维护事件列表、事件队列等。
 */
class WHPSPoller : public Poller
{
public:
        using OpsType = unsigned int;
        enum OPSTYPE
        {
                ADD = 0,
                DEL,
                UPDATE
        };
public:
        WHPSPoller(int maxevents, int timeout);
        ~WHPSPoller();

public:         // 外部调用
        /* 启动epoll任务 */
        int poll(Vector<event_chn*>& event_queue);

        /* 添加到事件通知列表 */
        void addEvent(event_chn* p_event);

        /* 从事件通知列表移除 */
        void delEvent(event_chn* p_event);

        /* 修改事件 */
        void updateEvent(event_chn* p_event);

private:        // 内部调用

        void eventOperation(OpsType type, event_chn* p_event);
        /* 新增一个事件到事件列表 */
        void addToEventList(int fd, event_chn* p_event);

        /* 从事件列表中删除一个事件 */
        void delFromEventList(int fd);

        /* 更新事件列表中已存在的一个事件 */
        // void updateEventList(int fd);

        /* 等待事件触发 */
        int epollWait();

        /* 向队列中添加已触发的事件，提交给外部线程处理 */
        void onEvent(Vector<event_chn*>& event_queue, const int& n_fds);
private:
        Map<int, event_chn*> _event_list;      // 事件列表 <fd, WHPSEventHandler*>
        SEplEvent _events[MAX_EVENTS];
};

#endif  // __WHPS_POLLER_H__
