#ifndef __WHPS_HTTP_SESSION_H__
#define __WHPS_HTTP_SESSION_H__

#include <string>

#include "WHPSTimer.h"
#include "WHPSTcpSession.h"
#include "WHPSHttpParser.h"
#include "HttpWhpsFactory.h"
#include "HttpWhps.h"   // for aplication layer call
#include "HttpWriterRegistser.h"
#include "WHPSThreadPool.h"

/* 系统静态资源处理器（不应对外可见，后面放到源文件里）
 */
class WhpsSysResource final: public HttpWhps
{
public:
        WhpsSysResource(const std::string& rootPath);

        virtual ~WhpsSysResource();

public:
        virtual void doGet(HttpWhpsRequest& request, HttpWhpsResponse& response);

        virtual void doPost(HttpWhpsRequest& request, HttpWhpsResponse& response)
        {
        }

private:
        /* 处理静态资源请求 */
        void getResouceFile(const std::string& path, HttpWhpsResponse& response, std::string& msg);

private:
        const std::string& _rootPath;   // 静态资源根目录
};

/* http session：单个http连接，基于tcp session的tcp连接，二者为强依赖关系
 * HttpSession的实例化，必须依赖于TcpSession；
 * 
 * （1）应用层的数据处理，入口应是：
 *      WHPSHttpSession::onHttpRecv()
 */
class WHPSHttpSession: public std::enable_shared_from_this<WHPSHttpSession>
{
public:
        using sp_TcpSession = WHPSTcpSession::sp_TcpSession;
        using sp_HttpSession = std::shared_ptr<WHPSHttpSession>;
        // using HttpSessionCB = std::function<void(const sp_HttpSession&)>;
        using HttpSessionCB = std::function<void()>;
        using HttpSessionCB_ = std::function<void(const sp_TcpSession&)>;
        using WriterFunc = HttpWriterRegistser::cbFunc;
        using HttpPtrType = HttpWhpsFactory::HttpPtrType;
        using TimerCallback_t = WHPSTimer::TimerCallback_t;
public:
        /* http session的实例化，必须依赖于tcp session，是一一对应的关系 */
        WHPSHttpSession(const sp_TcpSession _tcp_session, WHPSWorkerThreadPool& worker_thread_pool);

        ~WHPSHttpSession();

        /* 初始化各项参数，不与构造函数一起，防止乱序 */
        void init();

        /* 定时器回调函数 */
        void __stdcall TimerCallback(WHPSTimer& timer);

public:
        /* 获取当前http连接对应的tcp连接 */
        const sp_TcpSession& getTcpSession() const;

        /* 设置http连接回调函数，提供HttpServer当前释放的http session */
        void setHttpCloseCallback(HttpSessionCB_ cb);

private:
        /* http接收消息回调 */
        void onHttpMessage();

        /* 消息回调处理程序 */
        void onCallback(HttpRequestContext& request, HttpResponseContext& response);

        /* 静态资源请求处理 */
        void onStaticRequest(HttpRequestContext& request, HttpResponseContext& response);

        /* 动态资源请求处理 */
        void onDynamicRequest(HttpRequestContext& request, HttpResponseContext& response);

        /* http发送消息回调 */
        void onHttpSend();

        /* http连接关闭回调 */
        void onHttpClose();

        /* http异常错误回调 */
        void onHttpError();

private:
        /* http层根据Connection字段值，来判断是否通知tcp层
         * 发送数据后断开连接，作通知功能。
         */
        void notifyToClose();

        /* 关闭tcp层接口 */
        void closeAll();

private:
        // writer
        /* 客户端回写接口
         */
        void sendHttpMessage(const std::string& msg);

        void setConnStatus(int status)
        {
                std::lock_guard<std::mutex> lock(m_mutexStatus);
                m_connStatus = status;
        }

        const int& getConnStatus()
        {
                std::lock_guard<std::mutex> lock(m_mutexStatus);
                return m_connStatus;
        }

private:
        // 先使用这种方式测试http功能，后续引入工作线程池后，可以将发送消息的任务，加入到工作线程队列里
        const sp_TcpSession m_tcpSession;      // tcp连接对象
        HttpSessionCB_ m_httpCloseCB;            // http连接断开回调函数

        WHPSHttpParser m_httpParser;            // http解析器

private:
        const std::string& m_objName;           // 配置文件中，配置的子类名称
        WhpsSysResource m_whpsStaticProcessor; // 系统静态资源处理器
        HttpWhpsFactory* m_httpWhpsFactory;    // 应用层实例化工厂类对象
        // HttpWhps* _http_whps;                        
        HttpPtrType m_httpWhps;                 // 应用层（业务层）调用句柄

        WriterFunc m_writerFunc;
        HttpWriterRegistser m_Writer;   // 数据发送注册器

        TimerCallback_t m_cb;
        WHPSTimer m_Timer;

        enum ConnStatus         // 连接状态枚举
        {
                INIT = 0,       // 初始状态
                PROCESSING,     // 数据处理状态（有效状态）
                CLOSING,        // 缓冲状态（在此期间接收到数据，可恢复到有效状态）
                DISCONNECTED,   // 无效状态（释放资源、关闭连接等）
                STOPPED         // 停止状态（什么处理都不做）
        };

        int m_connStatus;             // 连接状态标志
        WHPSWorkerThreadPool& m_workerThreadPool;
        std::mutex m_mutexStatus;    // 数据处理标识锁
        std::mutex m_mutex;
};

#endif  // __WHPS_HTTP_SESSION_H__
