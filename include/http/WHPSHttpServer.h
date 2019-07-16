
#ifndef __WHPS_HTTP_SERVER_H__
#define __WHPS_HTTP_SERVER_H__

#include "ImplSingleton.h"
#include "WHPSTcpServer.h"

/* HTTP服务的核心类
 * 启动http服务、设置数据回调接口、创建httpsession等操作
 * 单例模式： 一个服务进程只能有一个HTTP服务
 */
class WHPSHttpServer : public ImplSingleton<WHPSHttpServer>
{
public:
        WHPSHttpServer();
        virtual ~WHPSHttpServer();

public:
        /* 获取单例对象句柄 */
        static WHPSHttpServer* Get();

        /* 启动http服务 */
        void start();

public:         // 测试接口
        void stop()
        {
                _tcp_server->stop();
                ImplSingleton<WHPSHttpServer>::free();
        }

private:

private:
        static WHPSHttpServer* _http_server;
        WHPSTcpServer* _tcp_server;
};

GET_SINGLETON_OBJECT(WHPSHttpServer)

#endif  //