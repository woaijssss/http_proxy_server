
#ifndef __WHPS_POLLER_H__
#define __WHPS_POLLER_H__

#include "WHPSEventHandler.h"
#include "Poller.h"

#include "map.h"

typedef WHPSEventHandler event_chn;

/* WHPS事件循环基础类，用于启动事件和对事件的增、删、改、查。
 * WHPSPoller中，维护事件列表、事件队列等。
 */
class WHPSPoller : public Poller
{
public:
        WHPSPoller();
        ~WHPSPoller();

public:
        /* 添加事件 */
        void addEvent(event_chn* p_event);

        /* 移除事件 */
        void delEvent(event_chn* p_event);

        /* 修改事件 */
        void updateEvent(event_chn* p_event);

private:
        /* 新增一个事件到事件列表 */
        void addToEventList(int fd, event_chn* p_event);

        /* 从事件列表中删除一个事件 */
        void delFromEventList(int fd);

        /* 更新事件列表中已存在的一个事件 */
        // void updateEventList(int fd);
private:
        Map<int, event_chn*> _event_list;      // 事件列表 <fd, WHPSEventHandler*>
};

#endif  // __WHPS_POLLER_H__