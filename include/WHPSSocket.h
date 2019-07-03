
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
        /* 初始化socket属性，确定socket模式 */
        int init();

        /* 服务器模式socket初始化 */
        int initServerMode();

        /* 连接客户端的socket初始化 */
        int initClientConnMode();

private:
        int _mode;
};

#endif 	// __WHPSSOCKET_H__