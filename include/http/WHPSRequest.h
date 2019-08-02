
#ifndef __WHPS_REQUEST_H__
#define __WHPS_REQUEST_H__

#include <map>
#include <string>

typedef std::map<std::string, std::string>  HttpHeader__;
#define HttpRequestHeader       HttpHeader__

/* http请求类实体 */
struct HttpRequestContext
{
        std::string         _method;        // 请求方法
        std::string         _url;           // 请求资源路径
        std::string         _version;       // 协议版本
        HttpRequestHeader   _header;        // 请求头(K-V格式)
        std::string         _body;          // 请求体
};

#endif  // __WHPS_REQUEST_H__