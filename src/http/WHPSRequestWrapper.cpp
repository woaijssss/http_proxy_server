#include "WHPSRequestWrapper.h"
#include "String.h"
#include "WHPSLog.h"

using namespace std;

WHPSRequestWrapper::WHPSRequestWrapper()
        : m_isStatic(false)
{

}

WHPSRequestWrapper::~WHPSRequestWrapper()
{

}

const string& WHPSRequestWrapper::_getMethod()
{
        return m_method;
}

const string& WHPSRequestWrapper::_getUrl()
{
        return m_url;
}

const string& WHPSRequestWrapper::_getUrlParams(const string& key)
{
        return m_url_params[key];
}

const string& WHPSRequestWrapper::_getVersion()
{
        return m_version;
}

HttpRequestHeader& WHPSRequestWrapper::_getHeader()
{
        return m_header;
}

const string& WHPSRequestWrapper::_getBody()
{
        return m_body;
}

const bool& WHPSRequestWrapper::_getFlag()
{
        return m_isStatic;
}

void WHPSRequestWrapper::_setMethod(const string& method)
{
        m_method = method;
}

void WHPSRequestWrapper::_seturl(const string& url)
{
        m_url = url;
}

void WHPSRequestWrapper::_setUrlParams(const std::string& url)
{
        String sUrl(url);
        size_t m = sUrl.count("?");

        if (m == 1)
        {
                m = sUrl.find('?');
                sUrl = sUrl.substr(m + 1, sUrl.size() - 1);
                String::SVector v_params = sUrl.split("&");

                for (auto& obj : v_params)
                {
                        String::SVector v_tmp = String(obj).split("=");
                        m_url_params[v_tmp[0]] = v_tmp[1];
                }
        }
}

void WHPSRequestWrapper::_setVersion(const string& version)
{
        m_version = version;
}

void WHPSRequestWrapper::_setHeader(const string& k, const string& v)
{
        m_header[k] = v;
}

void WHPSRequestWrapper::_setBody(const string& body)
{
        m_body = body;
}

void WHPSRequestWrapper::_setFlag(const bool& flag)
{
        m_isStatic = flag;
}
