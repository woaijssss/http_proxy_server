
#ifndef __WHPSTCPSOCKET_H__
#define __WHPSTCPSOCKET_H__

#include "WHPSSocket.h"

#include <iostream>

/* WHPS服务基础tcp socket类，所有的socket句柄都通过此类创建
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