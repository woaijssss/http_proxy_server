#include "WHPSTcpSocket.h"

WHPSTcpSocket::WHPSTcpSocket(int mode)
        : WHPSSocket(mode)
{

}

WHPSTcpSocket::~WHPSTcpSocket()
{

}

bool WHPSTcpSocket::isValid()
{
        return this->__isValid();
}

int WHPSTcpSocket::Accept(struct sockaddr_in& c_addr)
{
        return this->accept(c_addr);
}
