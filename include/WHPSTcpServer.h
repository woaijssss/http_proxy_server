
#ifndef __WHPSTCPSERVER_H__
#define __WHPSTCPSERVER_H__

#include "WHPSTcpSocket.h"
#include "WHPSEventHandler.h"
#include "WHPSEpollEventLoop.h"
#include "WHPSTcpSession.h"
#include "WHPSThreadPool.h"

#include <iostream>

/* Tcp服务类
 * 单例模式： 一个服务进程只能有一个主socket
 */
class WHPSTcpServer
{
public:
        using error_code = unsigned int;
public:
        /* 虽然是单例，但实例化了线程池，没有析构函数，服务停止时，无法正常释放线程资源，导致内存泄漏 */
        ~WHPSTcpServer();

        /* 获取单例实例 */
        static WHPSTcpServer* Get(int maxevents, int timeout);

        /* 应用侧检查句柄是否有效 */
        bool isValid();

        /* 获取服务器主socket */
        WHPSTcpSocket& getSocket();

        /* 启动tcp服务，包含：
         *      - 启动WHPSEpollEventLoop事件循环；
         *      - 设置epoll触发类型
         *      - 注册相关回调函数
         * 若启动失败(即：返回false)，则服务无效，重启服务即可。
         */
        bool start();

        /* 开启事件循环 */
        void startLoop();

        /* 获取主socket事件循环对象 */
        WHPSEpollEventLoop& loop();

private:
        /* 接收新建连接的回调函数 */
        void onNewConnection(error_code error);

        /* 处理新建连接，设置相关属性，并加入到_loop事件循环中 */
        void onNewSession();

public:
        /* 某个客户端连接关闭时，调用此回调清除句柄资源 */
        void onCleanUpResource(const sp_TcpSession& sp_tcp_session);

private:
        WHPSTcpServer(int maxevents, int timeout);
        static WHPSTcpServer* _tcp_server;

        WHPSThreadPool _thread_pool;    // 线程池句柄
        WHPSTcpSocket _tcp_socket;      // 服务器主socket
        WHPSEventHandler _event_chn;    // 服务器事件回调通道
        WHPSEpollEventLoop _loop;       // 服务器事件循环触发

        /* 保证在连接存在时，智能指针至少被引用一次，不至于销毁连接 */
        Map<int, sp_TcpSession> _tcp_sess_list;   // tcp客户端连接表(断线要清理)

        class GC            // 避免内存泄漏的垃圾回收(嵌套)类
        {
        public:
                ~GC()
                {
                        if (WHPSTcpServer::_tcp_server)
                        {
                                delete WHPSTcpServer::_tcp_server;
                        }
                }
        };
        static GC _gc; 
};

#endif  // __WHPSTCPSERVER_H__
