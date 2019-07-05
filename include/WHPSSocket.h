
#ifndef __WHPSSOCKET_H__
#define __WHPSSOCKET_H__

#include "Socket.h"

#define SERVER_MODE             0       // 服务器主socket
#define CLIENT_CONN_MODE        1       // 服务器接受连接的客户端的socket

/*
 * WHPS服务的基础socket类，初始化服务
 */

class WHPSSocket : public Socket
{
public:
        WHPSSocket(int mode);
        ~WHPSSocket();

public:
        /* 设置socket，适用于服务器接收的客户端socket */
        void set(int fd);

        /* 初始化socket属性，确定socket模式 */
        int init();

        /* 服务器模式socket初始化 */
        int initServerMode();

        /* 连接客户端的socket初始化 */
        int initClientConnMode();

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
        int _mode;
};

#endif 	// __WHPSSOCKET_H__