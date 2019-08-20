
#ifndef __WHPS_REQUEST_H__
#define __WHPS_REQUEST_H__

#include "WHPSRequestWrapper.h"

/* http请求类实体 */
struct HttpRequestContext: public WHPSRequestWrapper
{
public:
        HttpRequestContext();
        virtual ~HttpRequestContext();

public:
        /* 获取请求方法 */
        const std::string&    getMethod();

        /* 获取请求uri路径 */
        const std::string&    getUrl();

        /* 获取请求客户端的http版本信息 */
        const std::string&    getVersion();

        /* 获取所有请求消息头 */
        HttpRequestHeader&    getHeader();

        /* 获取请求消息体 */
        const std::string&    getBody();

        /* 获取当前请求的是否为静态资源标识 */
        const bool&           getFlag();

        void setMethod(const std::string& method);
        void setUrl(const std::string& url);
        void setVersion(const std::string& version);
        void setHeader(const std::string& k, const std::string& v);
        void setBody(const std::string& body);
        void setFlag(const bool& flag);
};

#endif  // __WHPS_REQUEST_H__