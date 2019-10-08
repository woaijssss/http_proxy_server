
#ifndef __WHPS_REQUEST_WRAPPER_H__
#define __WHPS_REQUEST_WRAPPER_H__

#include <map>
#include <string>

typedef std::map<std::string, std::string>  HttpHeader__;
typedef std::string                         HttpBody__;
#define HttpRequestHeader       HttpHeader__
#define HttpRequestBody         HttpBody__

class WHPSRequestWrapper
{
public:
        WHPSRequestWrapper();
        virtual ~WHPSRequestWrapper();
protected:
        /* 获取请求方法 */
        const std::string&       _getMethod();

        /* 获取请求uri路径 */
        const std::string&       _getUrl();

        /* 获取请求客户端的http版本信息 */
        const std::string&       _getVersion();

        /* 获取所有请求消息头 */
        HttpRequestHeader&       _getHeader();

        /* 获取请求消息体 */
        const std::string&       _getBody();

        /* 获取当前请求的是否为静态资源标识 */
        const bool&              _getFlag();

protected:
        void _setMethod(const std::string& method);
        void _seturl(const std::string& url);
        void _setVersion(const std::string& version);
        void _setHeader(const std::string& k, const std::string& v);
        void _setBody(const std::string& body);
        void _setFlag(const bool& flag);
        
private:
        std::string             _method;        // 请求方法
        std::string             _url;           // 请求资源路径
        std::string             _version;       // 协议版本
        HttpRequestHeader       _header;        // 请求头(K-V格式)
        std::string             _body;          // 请求体

        bool                    _isStatic;      // 是否是静态资源
};

#endif  // __WHPS_REQUEST_WRAPPER_H__
