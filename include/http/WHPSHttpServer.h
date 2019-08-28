
#ifndef __WHPS_HTTP_SERVER_H__
#define __WHPS_HTTP_SERVER_H__

#include "ImplSingleton.h"
#include "WHPSTcpServer.h"
#include "WHPSHttpSession.h"

/* HTTP服务的核心类
 * 启动http服务、设置数据回调接口、创建httpsession等操作
 * 单例模式： 一个服务进程只能有一个HTTP服务
 */
class WHPSHttpServer : public ImplSingleton<WHPSHttpServer>
{
public:
        using sp_TcpSession = WHPSTcpServer::sp_TcpSession;
        using sp_HttpSession = WHPSHttpSession::sp_HttpSession;
public:
        WHPSHttpServer();
        virtual ~WHPSHttpServer();

public:
        /* 获取单例对象句柄 */
        static WHPSHttpServer& GetInstance();

        /* 启动http服务 */
        void start();

public:         // 测试接口
        void stop()
        {
                _tcp_server->stop();
        }

private:        // 业务函数
        /* 接收新建连接的回调函数 */
        void onNewConnection(sp_TcpSession tcp_session);

        /* 处理新建连接，创建http session，并设置tcp session对应于http session的回调函数
         * 应由 onNewConnection() 内部调用
         */
        void onNewSession(sp_TcpSession tcp_session);

        /* 连接断开回调函数 */
        void onNewClose(sp_TcpSession tcp_session);

public:     // 后面需要增加一些接口，用来进行外部监控（比如：当前http有效连接数等）

private:
        // static WHPSHttpServer* _http_server;
        static std::shared_ptr<WHPSHttpServer> _http_server;
        WHPSTcpServer* _tcp_server;

        Map<std::string, sp_HttpSession> _http_sess_list;    // http列表
        WHPSWorkerThreadPool _worker_thread_pool;       // 工作线程池句柄
};

GET_SINGLETON_OBJECT(WHPSHttpServer)

#endif  //
