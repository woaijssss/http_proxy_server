#ifndef __HTTP_WHPS_H__
#define __HTTP_WHPS_H__

#include "WHPSHttpParser.h"

typedef struct HttpRequestContext HttpWhpsRequest;        // http请求信息
typedef struct HttpResponseContext HttpWhpsResponse;       // http响应信息

/* WHPS http服务最上层（业务层）虚类
 * 所有实现http应用开发，都要继承于此，通过配置文件来配置以下信息：
 *      （1）类文件路径
 */
class HttpWhps
{
public:
        HttpWhps()
        {

        }

        virtual ~HttpWhps()
        {

        }

public:
        /* GET/DELETE 方法的请求处理 */
        virtual void doGet(HttpWhpsRequest& request, HttpWhpsResponse& response) = 0;

        /* POST/PUT 方法的请求处理 */
        virtual void doPost(HttpWhpsRequest& request, HttpWhpsResponse& response) = 0;
        // void doPut(HttpWhpsRequest request, HttpWhpsResponse response);
        // void doDelete(HttpWhpsRequest request, HttpWhpsResponse response);
private:
};

#endif  // __HTTP_WHPS_H__
