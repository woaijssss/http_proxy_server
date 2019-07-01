
#ifndef __WHPSSOCKET_H__
#define __WHPSSOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>

class WHPSSocket
{
public:
        WHPSSocket()
        ~WHPSSocket();

public:

private:
        int _socket_fd;
};

#endif 	// __WHPSSOCKET_H__