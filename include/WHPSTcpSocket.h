#ifndef __WHPSTCPSOCKET_H__
#define __WHPSTCPSOCKET_H__

#include "WHPSSocket.h"
// #include "WHPSConnSocket.h"

#include <iostream>

/* WHPS服务主socket类
 */
class WHPSTcpSocket: public WHPSSocket
{
public:
        WHPSTcpSocket(int mode);
        virtual ~WHPSTcpSocket();

        /* 应用侧检查句柄是否有效 */
        bool isValid();

        /* 主socket接收客户端连接
         * 返回一个客户端socket对象
         */
        int Accept(struct sockaddr_in& c_addr);
private:
};

#endif  //__WHPSTCPSOCKET_H__
