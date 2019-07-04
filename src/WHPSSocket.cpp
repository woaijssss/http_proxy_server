
#include "WHPSSocket.h"

int g_port = 22;              // 可做成配置项

WHPSSocket::WHPSSocket(int mode)
        : _mode(mode)
{
        this->init();
}

WHPSSocket::~WHPSSocket()
{

}

int WHPSSocket::init()
{
        if (_mode == SERVER_MODE)
        {
                return this->initServerMode();
        }
        else if(_mode == CLIENT_CONN_MODE)
        {
                return this->initClientConnMode();
        }

        return -1;      // 未知服务类型
}

int WHPSSocket::initServerMode()
{
        if (this->setReuseAddr())
        {
                return -1;
        }

        if (bind(g_port))
        {
                return -1;
        }

        if (listen())
        {
                return -1;
        }

        return 0;
}

int WHPSSocket::initClientConnMode()
{
        if (this->__isValid())
        {
                return 0;
        }
        else
        {
                return -1;
        }
}