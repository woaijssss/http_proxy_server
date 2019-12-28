#ifndef __WHPSSOCKET_H__
#define __WHPSSOCKET_H__

#include "Socket.h"
#include "WHPSLog.h"

#define SERVER_HTTP_MODE             0       // http服务模式
#define SERVER_HTTPS_MODE             1       // https服务模式
/*
 * WHPS服务的基础socket类，初始化服务
 */

class WHPSSocket: public Socket
{
public:
        WHPSSocket(int port);
        virtual ~WHPSSocket();

public:
        /* 设置socket，适用于服务器接收的客户端socket */
        void set(int fd);

        /* 初始化socket属性 */
        int init();

        /* 获取当前socket句柄 */
        const int& get() const;

protected:

        /* 设置socket属性 */
        int setSocketOpt();

        /* 设置非阻塞socket */
        int setNonblock();

        /* 创建一个socket */
        int socket();

        /* 绑定一个端口
         *      server模式：ip地址无需传递
         *      client模式：ip地址需传指定的服务器ip
         */
        int bind(const int& port, const char* ip = NULL);

        /* 启动监听 */
        int listen();

        /* 获取被监听的连接请求 */
        int accept();   // 暂时不用
        int accept(struct sockaddr_in& c_addr);         // 优先实现这个接口
        //struct sockaddr_in accpet();    // 暂时不用

        /* 设置socket地址重用(主要用于服务端socket) */
        int setReuseAddr();

        /* 关闭socket */
        int close();

private:
//        int m_mode;
        int m_tcpPort;   // tcp监听端口
};

#endif 	// __WHPSSOCKET_H__
