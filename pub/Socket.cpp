#include <stdio.h>      // for NULL
// for fcntl
#include <unistd.h>
#include <fcntl.h>
// -----------------

#include <string.h>     // for memset
#include <unistd.h>
#include <stdlib.h>

#include "Socket.h"

Socket::Socket()
        : m_socketFd(INVALID_SOCK)
{

}

Socket::~Socket()
{
        if (m_socketFd != INVALID_SOCK)
        {
                this->__close();
                m_socketFd = INVALID_SOCK;
        }
}

void Socket::__set(int fd)
{
        m_socketFd = fd;
}

#include <netinet/tcp.h> // for TCP_NODELAY
int Socket::__setSocketOpt()
{
        int on = 1;
        setsockopt(m_socketFd, SOL_SOCKET, SO_KEEPALIVE, (void*) &on, sizeof(on));
        setsockopt(m_socketFd, IPPROTO_TCP, TCP_NODELAY, (void*) &on, sizeof(on));
        setsockopt(m_socketFd, SOL_TCP, TCP_NODELAY, (void*) &on, sizeof(on));

        return 0;
}

const int &Socket::__get() const
{
        return m_socketFd;
}

bool Socket::__isValid()
{
        if (m_socketFd == INVALID_SOCK)         // 无效的s
        {
                return false;
        }

        return true;
}

int Socket::__setNonblock()
{
        int flag = fcntl(m_socketFd, F_GETFL, 0);

        if (flag >= 0)
        {
                if (fcntl(m_socketFd, F_SETFL, flag | O_NONBLOCK) >= 0)
                {
                        return 0;
                }
        }

        return -1;
}

int Socket::__socket()
{
        m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
        return m_socketFd;
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
        } else            // 服务器绑定地址
        {
                so_addr.sin_addr.s_addr = htonl(INADDR_ANY );
        }

        int res = bind(m_socketFd, (struct sockaddr*) &so_addr, sizeof(so_addr));

        return res;
}

int Socket::__listen()
{
        return listen(m_socketFd, LISTEN_SIZE);
}

/* 暂时不用 */
int Socket::__accept()
{
        return 0;
}

int Socket::__accept(struct sockaddr_in &c_addr)
{
        socklen_t addr_len = sizeof(c_addr);
        return accept(m_socketFd, (struct sockaddr*) &c_addr, &addr_len);
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
        return setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
}

int Socket::__close()
{
        if (!close(m_socketFd))         // 关闭成功
        {
                m_socketFd = INVALID_SOCK;
                return 0;
        }

        return -1;                      // 关闭失败
}
