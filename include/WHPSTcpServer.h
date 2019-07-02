
#ifndef __WHPSTCPSERVER_H__
#define __WHPSTCPSERVER_H__

#include "WHPSTcpSocket.h"

#include <iostream>

/* Tcp服务类
 * 单例模式： 一个服务进程只能有一个主socket
 */
class WHPSTcpServer
{
public:
        /* 获取单例实例 */
        static WHPSTcpServer* Get();

        /* 应用侧检查句柄是否有效 */
        bool isValid();

        WHPSTcpSocket& getSocket();

private:
        WHPSTcpServer();
        static WHPSTcpServer* _tcp_server;

        class GC            // 避免内存泄漏的垃圾回收(嵌套)类
        {
        public:
                ~GC()
                {
                        if(WHPSTcpServer::_tcp_server) 
                        {
                                delete WHPSTcpServer::_tcp_server;
                        }
                }
        };

        static GC _gc; 

        WHPSTcpSocket _tcp_socket; 
};

#endif  // __WHPSTCPSERVER_H__