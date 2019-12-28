#ifndef __WHPS_TCP_SESSION_H__
#define __WHPS_TCP_SESSION_H__

#include <memory>
#include <mutex>

#include <openssl/ssl.h>

#include "WHPSConnSocket.h"
#include "WHPSEventHandler.h"
#include "WHPSEpollEventLoop.h"
#include "WHPSSslManager.h"

/* 连接客户端表示类，该类实例化的每个对象，分别表示一个客户端的一条tcp连接
 * 句柄的操作由WHPSTcpConnSocket来维护；
 * 该类不做socket的改变
 */
class WHPSTcpSession: public std::enable_shared_from_this<WHPSTcpSession>
{
public:
        using error_code = unsigned int;
        using sp_TcpSession = std::shared_ptr<WHPSTcpSession>;
        using TcpSessionCB = std::function<void(const sp_TcpSession&)>;

        // 应用层回调函数类型
        using httpCB = std::function<void()>;
public:
        WHPSTcpSession(WHPSEpollEventLoop& loop, const int& fd, struct sockaddr_in& c_addr, WHPSSslManager& sslMgr);
        ~WHPSTcpSession();

        /* 初始化各项参数，不与构造函数一起，防止乱序 */
        void init();

        std::string& getBufferIn()
        {
                return m_bufferIn;
        }

        const std::string& getBufferOut()
        {
                return m_bufferOut;
        }

        WHPSEpollEventLoop& getLoop()
        {
                return m_loop;
        }

        const bool& getConnectFlag()
        {
                std::lock_guard<std::mutex> lock(m_connFlagMutex);

                return m_isConnect;
        }

        void setConnectFlag()
        {
                std::lock_guard<std::mutex> lock(m_connFlagMutex);
                m_isConnect = false;
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
        void closeSession();

        /* 清除当前session的所有资源
         * 包括：
         * （1）所有回调函数指针置空
         * （2）socket关闭
         * （3）buffer缓存清空
         */
        void release();

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

public:
        // 应用层回调函数设置
        /* http接收数据回调函数 */
        void setHttpMessageCallback(httpCB cb);

        /* http发送数据回调函数 */
        void setHttpSendCallback(httpCB cb);

        /* http连接关闭回调函数 */
        void setHttpCloseCallback(httpCB cb);

        /* http异常错误回调函数 */
        void setHttpErrorCallback(httpCB cb);

        void setProcessingFlag(bool is_processing);

        const bool& getProcessingFlag();

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

        /* 读取ssl数据 */
        int readTcpSslMessage(std::string& buffer_in);
        int sendTcpSslMessage(std::string& buffer_out);
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

        void onCall(httpCB cb);
        /**********************************************************************/

private:
        // 网络层各对象
        struct sockaddr_in m_cAddr;     // 保存客户端的连接信息
        WHPSEpollEventLoop& m_loop;      // 引用外部事件循环
        WHPSConnSocket m_connSock;      // 连接句柄——tcp通道
        WHPSSslManager& m_sslMgr; // ssl管理器
        bool m_useSsl;          // 是否使用ssl通道
        SSL* m_ssl;                     // 连接句柄——ssl通道

        event_chn m_eventChn;           // 服务器事件回调通道
        events_t m_baseEvents;         // 连接默认的事件监听标识

        TcpSessionCB m_cbCleanup;

private:
        // 对应用服务层提供的可用数据
        std::string m_clientIp;         // 客户端ip
        int m_clientPort;               // 客户端端口
        std::string m_netInfo;          // 客户端网络信息(格式：   ip:port)

        std::string m_bufferIn;         // 接收消息缓冲(tcp请求)
        std::string m_bufferOut;        // 发送消息缓冲(tcp响应)

        bool m_isConnect /*= true*/;    // 连接标志(默认为true)
        bool m_isStop /*= false*/;			//

        bool m_isWait;

private:
        // 应用层回调函数定义
        httpCB m_httpOnMessage;            // http接收数据回调函数
        httpCB m_httpOnSend;            // http发送数据回调函数
        httpCB m_httpOnClose;           // http连接关闭回调函数
        httpCB m_httpOnError;           // http异常错误回调函数

        std::mutex m_mutex;
        std::mutex m_connFlagMutex;
        std::mutex m_sslMutex;
};

typedef WHPSTcpSession::TcpSessionCB TcpSessionCB;

#endif  // __WHPS_TCP_SESSION_H__
