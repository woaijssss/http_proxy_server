
#include <stdio.h>      // for NULL

// for fcntl
#include <unistd.h>
#include <fcntl.h>
// -----------------

#include <string.h>     // for memset
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

#include "Socket.h"

Socket::Socket()
        : __socket_fd(INVALID_SOCK)
{
        
}

Socket::~Socket()
{
        if (__socket_fd != INVALID_SOCK)
        {
                this->__close();
                __socket_fd = INVALID_SOCK;
        }
}

void Socket::__set(int fd)
{
        __socket_fd = fd;
}

int Socket::__setSocketOpt()
{
        return 0;
}

const int &Socket::__get() const
{
        return __socket_fd;
}

bool Socket::__isValid()
{
        if (__socket_fd == INVALID_SOCK)         // 无效的s
        {
                return false;
        }

        return true;
}

int Socket::__setNonblock()
{
        int flag = fcntl(__socket_fd, F_GETFL, 0);

        if (flag >= 0)
        {
                if (fcntl(__socket_fd, F_SETFL, flag | O_NONBLOCK) >= 0)
                {
                        return 0;
                }
        }

        return -1;
}

int Socket::__socket()
{
        __socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        return __socket_fd;
}

int Socket::__bind(const int &port, const char *ip)
{
        struct sockaddr_in so_addr;
        memset(&so_addr, 0, sizeof(so_addr));
        so_addr.sin_family = AF_INET;   // 设置地址族
        so_addr.sin_port = htons(port);         // 绑定端口

        if (ip)        // 客户端绑定地址
        {
                so_addr.sin_addr.s_addr = inet_addr(ip);
        }
        else            // 服务器绑定地址
        {
                so_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        int res = bind(__socket_fd, (struct sockaddr*)&so_addr, sizeof(so_addr));

        return res;
}

int Socket::__listen()
{
        return listen(__socket_fd, LISTEN_SIZE);
}

/* 暂时不用 */
int Socket::__accept()
{
        return 0;
}

int Socket::__accept(struct sockaddr_in &c_addr)
{
        socklen_t addr_len = sizeof(c_addr);
        return accept(__socket_fd, (struct sockaddr*)&c_addr, &addr_len);
}

/* 暂时不用 */
struct sockaddr_in Socket::__accpet()
{
        struct sockaddr_in clientaddr;
        return clientaddr;
}

int Socket::__setReuseAddr()
{
        int v = 1;
        return setsockopt(__socket_fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
}

int Socket::__close()
{
        if (!close(__socket_fd))         // 关闭成功
        {
                __socket_fd = INVALID_SOCK;
                return 0;
        }

        return -1;                      // 关闭失败
}
