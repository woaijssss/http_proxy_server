
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
        : _socket_fd(INVALID_SOCK)
{
        _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket()
{
        if (_socket_fd != INVALID_SOCK)
        {
                this->close();
        }
}

int Socket::setSocketOpt()
{
        return 0;
}

const int &Socket::get() const
{
        return _socket_fd;
}

bool Socket::__isValid()
{
        if (_socket_fd == INVALID_SOCK)         // 无效的s
        {
                return false;
        }

        return true;
}

int Socket::setNonblock()
{
        int opts = fcntl(_socket_fd, F_GETFL);

        if (opts >= 0)
        {
                if (fcntl(_socket_fd, F_SETFL, opts | O_NONBLOCK) >= 0)
                {
                        return 0;
                }
        }

        return -1;
}

int Socket::bind(const int &port, const char *ip)
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

        int res = ::bind(_socket_fd, (struct sockaddr*)&so_addr, sizeof(so_addr));

        return res;
}

int Socket::listen()
{
        return ::listen(_socket_fd, LISTEN_SIZE);
}

/* 暂时不用 */
int Socket::accept()
{
        return 0;
}

int Socket::accept(struct sockaddr_in &c_addr)
{
        socklen_t addr_len = sizeof(c_addr);
        return ::accept(_socket_fd, (struct sockaddr*)&c_addr, &addr_len);
}

/* 暂时不用 */
struct sockaddr_in Socket::accpet()
{
        struct sockaddr_in clientaddr;
        return clientaddr;
}

int Socket::setReuseAddr()
{
        int v = 0;
        return setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
}

int Socket::close()
{
        if (!::close(_socket_fd))         // 关闭成功
        {
                _socket_fd = INVALID_SOCK;
                return 0;
        }

        return -1;                      // 关闭失败
}