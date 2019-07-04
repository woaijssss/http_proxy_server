
#ifndef __WHPSTCPSERVER_H__
#define __WHPSTCPSERVER_H__

#include "WHPSTcpSocket.h"
#include "WHPSEventHandler.h"
#include "WHPSEpollEventLoop.h"

#include <iostream>

/* Tcp服务类
 * 单例模式： 一个服务进程只能有一个主socket
 */
class WHPSTcpServer
{
public:
        using error_code = unsigned int;
public:
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

        WHPSEpollEventLoop& loop();

private:
        void onAccept(error_code error);

private:
        WHPSTcpServer(int maxevents, int timeout);
        static WHPSTcpServer* _tcp_server;

        WHPSTcpSocket _tcp_socket;      // 服务器主socket
        WHPSEventHandler _event_chn;    // 服务器事件回调通道
        WHPSEpollEventLoop _loop;       // 服务器事件循环触发

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
