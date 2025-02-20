#include <iostream>
#include "WHPSSocket.h"
#include "WHPSConfig.h"

using namespace std;

// int g_port = 3000;              // 可做成配置项

WHPSSocket::WHPSSocket(int port)
        : m_tcpPort(port)
{

}

WHPSSocket::~WHPSSocket()
{

}

const int& WHPSSocket::get() const
{
        return this->__get();
}

int WHPSSocket::init()
{
        if (this->socket() < 0)
        {
                WHPSLogError("create socket failed...");
                return -1;
        }

        if (this->setNonblock())
        {
                WHPSLogError("setNonblock failed...");
                return -1;
        }

        if (this->setReuseAddr())
        {
                WHPSLogError("setReuseAddr failed...");
                return -1;
        }

        if (this->bind(m_tcpPort))
        {
                WHPSLogError("bind failed...");
                return -1;
        }

        if (this->listen())
        {
                WHPSLogError("listen failed...");
                return -1;
        }

        return 0;
}

void WHPSSocket::set(int fd)
{
        this->__set(fd);
}

/* 设置socket属性 */
int WHPSSocket::setSocketOpt()
{
        return this->__setSocketOpt();
}

/* 设置非阻塞socket */
int WHPSSocket::setNonblock()
{
        return this->__setNonblock();
}

int WHPSSocket::socket()
{
        return this->__socket();
}

int WHPSSocket::bind(const int& port, const char* ip)
{
        return this->__bind(port, ip);
}

int WHPSSocket::listen()
{
        return this->__listen();
}

int WHPSSocket::accept()
{
        return this->__accept();
}

int WHPSSocket::accept(struct sockaddr_in& c_addr)
{
        return this->__accept(c_addr);
}

#if 0
struct sockaddr_in WHPSSocket::accpet()
{
        return this->__accept();
}
#endif

int WHPSSocket::setReuseAddr()
{
        return this->__setReuseAddr();
}

int WHPSSocket::close()
{
        return this->__close();
}
