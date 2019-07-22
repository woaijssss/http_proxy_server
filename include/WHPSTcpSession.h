
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
        using TcpSessionCB = std::function<void(const sp_TcpSession&)>;

        // 应用层回调函数类型
        using httpCB = std::function<void(const sp_TcpSession&)>;
public:
        WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr);
        ~WHPSTcpSession();

        std::string& getBufferIn()
        {
                return _buffer_in;
        }

        const std::string& getBufferOut()
        {
                return _buffer_out;
        }

public:
        /* 获取对端网络信息：
         *    ip、端口等
         */
        void getEndpointInfo();

        /* 获取客户端ip */
        const std::string& getIp() const;

        /* 获取客户端端口 */
        const int& getPort() const;

        /* 获取客户端网络信息
         * 格式为：
         *      ip:port
         */
        const std::string& getNetInfo() const;


        /* 检查句柄是否有效 */
        bool isValid();

        /* 获取当前socket句柄 */
        const int& get() const;

        /* 获取当前连接信息句柄 */
        WHPSConnSocket& getConn();

        /* 主动关闭连接 */
        void close();

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

        /* 写数据要触发epoll事件 */
        void send(const std::string& msg);

public:         // 应用层回调函数设置
        /* http接收数据回调函数 */
        void setHttpMessageCallback(httpCB cb);

        /* http发送数据回调函数 */
        void setHttpSendCallback(httpCB cb);

        /* http连接关闭回调函数 */
        void setHttpCloseCallback(httpCB cb);

        /* http异常错误回调函数 */
        void setHttpErrorCallback(httpCB cb);

        void setProcessingFlag(bool is_processing);

        const bool& getProcessingFlag() const;

private:

        /**********************************************************************/
                // 当前读、写异常情况处理可能不全面，后续出问题再追加
        /* 写数据要触发epoll事件，必须要主动先写一次，调用此接口
         * 由send()调用
         */
        int sendTcpMessage(std::string& buffer_out);

        /* 真正的读read()操作，由onNewRead() epoll事件触发后调用 
         * 由onNewRead()调用
         */
        int readTcpMessage(std::string& buffer_in);
        /**********************************************************************/

        /**********************************************************************/
                // error_code字段值为预留，后续扩展要加入异常
        /* 接收数据事件处理回调 */
        void onNewRead(error_code error);

        /* 发送数据事件处理回调 
         * 该接口的实现中，应检测数据是否发送完毕，并将剩余数据递归发送完成。
         */
        void onNewWrite(error_code error);

        /* 关闭连接事件处理回调 */
        void onNewClose(error_code error);

        /* 异常错误事件处理回调 */
        void onNewError(error_code error);
        /**********************************************************************/

private:
        struct sockaddr_in _c_addr;     // 保存客户端的连接信息
        WHPSEpollEventLoop& _loop;      // 引用外部事件循环
        WHPSConnSocket _conn_sock;      // 连接句柄
        event_chn _event_chn;           // 服务器事件回调通道
        events_t  _base_events;         // 连接默认的事件监听标识

        TcpSessionCB _cb_cleanup;

private:    // 对应用服务层提供的可用数据
        std::string _client_ip;         // 客户端ip
        int _client_port;               // 客户端端口
        std::string _net_info;          // 客户端网络信息(格式：   ip:port)
        
        std::string _buffer_in;         // 接收消息缓冲(tcp请求)
        std::string _buffer_out;        // 发送消息缓冲(tcp响应)

        bool _is_connect /*= true*/;    // 连接标志(默认为true)

        bool _is_processing;             // 数据处理标志
        bool _is_wait;

private:        // 应用层回调函数定义
        httpCB _http_onMessage;            // http接收数据回调函数
        httpCB _http_onSend;            // http发送数据回调函数
        httpCB _http_onClose;           // http连接关闭回调函数
        httpCB _http_onError;           // http异常错误回调函数
};

typedef WHPSTcpSession::TcpSessionCB    TcpSessionCB;

#endif  // __WHPS_TCP_SESSION_H__
