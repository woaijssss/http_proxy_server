
#include "WHPSTcpSocket.h"

WHPSTcpSocket::WHPSTcpSocket(int mode)
        : WHPSSocket(mode)
{
        this->init();
}

WHPSTcpSocket::~WHPSTcpSocket()
{
        
}

bool WHPSTcpSocket::isValid()
{
        return this->__isValid();
}