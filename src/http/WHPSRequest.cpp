
#include "WHPSRequest.h"

using namespace std;

HttpRequestContext::HttpRequestContext()
{

}

HttpRequestContext::~HttpRequestContext()
{

}

const string& HttpRequestContext::getMethod()
{
        return this->_getMethod();
}

const string& HttpRequestContext::getUrl()
{
        return this->_getUrl();
}

const string& HttpRequestContext::getVersion()
{
        return this->_getVersion();
}

HttpRequestHeader& HttpRequestContext::getHeader()
{
        return this->_getHeader();
}

const string& HttpRequestContext::getBody()
{
        return this->_getBody();
}

const bool& HttpRequestContext::getFlag()
{
        return this->_getFlag();
}

void HttpRequestContext::setMethod(const std::string& method)
{
        this->_setMethod(method);
}

void HttpRequestContext::setUrl(const std::string& url)
{
        this->_seturl(url);
}

void HttpRequestContext::setVersion(const std::string& version)
{
        this->_setVersion(version);
}

void HttpRequestContext::setHeader(const std::string& k, const std::string& v)
{
        this->_setHeader(k, v);
}

void HttpRequestContext::setBody(const std::string& body)
{
        this->_setBody(body);
}

void HttpRequestContext::setFlag(const bool& flag)
{
        this->_setFlag(flag);
}
