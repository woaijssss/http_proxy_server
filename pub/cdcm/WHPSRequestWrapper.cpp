
#include "WHPSRequestWrapper.h"
#include "String.h"
#include "WHPSLog.h"

using namespace std;

WHPSRequestWrapper::WHPSRequestWrapper()
        : _isStatic(false)
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

const string& WHPSRequestWrapper::_getUrlParams(const string& key)
{
        return _url_params.get(key);
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

void WHPSRequestWrapper::_setUrlParams(const std::string& url)
{
        String sUrl(url);
        size_t m = sUrl.count("?");

        if (m == 1)
        {
                m = sUrl.find('?');
                sUrl = sUrl.substr(m+1, sUrl.size()-1);
                String::SVector v_params = sUrl.split("&");

                for (auto& obj: v_params)
                {
                        String::SVector v_tmp = String(obj).split("=");
                        _url_params[v_tmp[0]] = v_tmp[1];
                }
        }
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
