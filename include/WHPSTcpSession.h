
#ifndef __WHPS_TCP_SESSION_H__
#define __WHPS_TCP_SESSION_H__

#include <memory>

#include "WHPSConnSocket.h"
#include "WHPSEventHandler.h"
#include "WHPSEpollEventLoop.h"

/* 连接客户端表示类，该类实例化的每个对象，分别表示一个客户端的一条tcp连接
 * 句柄的操作由WHPSTcpConnSocket来维护；
 * 该类不做socket的改变
 */
class WHPSTcpSession : public std::enable_shared_from_this<WHPSTcpSession>
{
public:
        using error_code = unsigned int;
        using sp_TcpSession = std::shared_ptr<WHPSTcpSession>;
        using TcpSessionCB = std::function<void(const sp_TcpSession&)> ;
public:
        WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr);
        ~WHPSTcpSession();

public:
        /* 检查句柄是否有效 */
        bool isValid();

        /* 获取当前socket句柄 */
        const int& get() const;

        /* 获取当前连接信息句柄 */
        WHPSConnSocket& getConn();

        /* 将当前的客户端session的事件回调通道，加入到EventLoop事件循环中 */
        void addToEventLoop();

        /* 移除事件 */
        void delFromEventLoop();

        /* 设置资源清理回调函数
         * 由于socket是从主socket申请的资源，主socket和EventLoop都有记录，
         * 因此，需要设置该回调函数。
         * 在socket触发close()时，该函数会回调到主socket中，进而清理socket资源。
         */
        void setCleanUpCallback(TcpSessionCB& cb);

private:
        /* 接收数据事件处理回调 */
        void onNewRead(error_code error);

        /* 关闭连接事件处理回调 */
        void onNewClose(error_code error);

private:
        struct sockaddr_in _c_addr;     // 保存客户端的连接信息
        WHPSEpollEventLoop& _loop;      // 引用外部事件循环
        WHPSConnSocket _conn_sock;      // 连接句柄
        event_chn _event_chn;           // 服务器事件回调通道

        TcpSessionCB _cb_cleanup;
};

typedef WHPSTcpSession::sp_TcpSession   sp_TcpSession;
typedef WHPSTcpSession::TcpSessionCB    TcpSessionCB;

#endif  // __WHPS_TCP_SESSION_H__
