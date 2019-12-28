#include "WHPSTcpSocket.h"

WHPSTcpSocket::WHPSTcpSocket(int port)
        : WHPSSocket(port)
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
