
#ifndef __WHPS_CONN_SOCKET_H__
#define __WHPS_CONN_SOCKET_H__

#include "Socket.h"

/* 客户端连接socket类，保留客户端的连接信息
 */
class WHPSConnSocket : public Socket
{
public:
        //WHPSConnSocket();
        WHPSConnSocket(int fd);
        virtual ~WHPSConnSocket();

public:
        /* 检查句柄是否有效 */
        bool isValid();

        /* 获取当前socket句柄 */
        const int& get() const;

        /* 设置客户端socket，用于在主socket Accept后设置 */
        void set(int fd);

        /* 设置非阻塞socket */
        int setNonblock();

        /* 设置socket属性 */
        int setOption();

        /* 关闭socket */
        int close();

private:
};

#endif  // __WHPS_CONN_SOCKET_H__
