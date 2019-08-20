
#include "WHPSRequestWrapper.h"

using namespace std;

WHPSRequestWrapper::WHPSRequestWrapper()
{

}

WHPSRequestWrapper::~WHPSRequestWrapper()
{

}

const string& WHPSRequestWrapper::_getMethod()
{
        return _method;
}

const string& WHPSRequestWrapper::_getUrl()
{
        return _url;
}

const string& WHPSRequestWrapper::_getVersion()
{
        return _version;
}

HttpRequestHeader& WHPSRequestWrapper::_getHeader()
{
        return _header;
}

const string& WHPSRequestWrapper::_getBody()
{
        return _body;
}

const bool& WHPSRequestWrapper::_getFlag()
{
        return _isStatic;
}

void WHPSRequestWrapper::_setMethod(const string& method)
{
        _method = method;
}

void WHPSRequestWrapper::_seturl(const string& url)
{
        _url = url;
}

void WHPSRequestWrapper::_setVersion(const string& version)
{
        _version = version;
}

void WHPSRequestWrapper::_setHeader(const string& k, const string& v)
{
        _header[k] = v;
}

void WHPSRequestWrapper::_setBody(const string& body)
{
        _body = body;
}

void WHPSRequestWrapper::_setFlag(const bool& flag)
{
        _isStatic = flag;
}
