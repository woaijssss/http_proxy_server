#include "WHPSConnSocket.h"

#include <iostream>
using namespace std;

#if 0
WHPSConnSocket::WHPSConnSocket()
{

}
#endif

WHPSConnSocket::WHPSConnSocket(int fd)
{
        this->set(fd);
}

WHPSConnSocket::~WHPSConnSocket()
{
        this->close();
}

bool WHPSConnSocket::isValid()
{
        return this->__isValid();
}

const int& WHPSConnSocket::get() const
{
        return this->__get();
}

void WHPSConnSocket::set(int fd)
{
        this->__set(fd);
}

int WHPSConnSocket::setNonblock()
{
        return this->__setNonblock();
}

int WHPSConnSocket::setOption()
{
        return this->__setSocketOpt();
}

int WHPSConnSocket::close()
{
        if (this->isValid())
        {
                return this->__close();
        }

        return 0;
}
