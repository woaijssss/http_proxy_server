
#ifndef __WHPSTCPSOCKET_H__
#define __WHPSTCPSOCKET_H__

#include "WHPSSocket.h"

#include <iostream>

/* 服务器主socket类
 * 单例模式： 一个服务进程只能有一个主socket
 */
class WHPSTcpSocket : public WHPSSocket
{
public:
        WHPSTcpSocket(int mode);
        ~WHPSTcpSocket();

        /* 应用侧检查句柄是否有效 */
        bool isValid();

private:
};

#endif  //__WHPSTCPSOCKET_H__