
#include "WHPSResponse.h"

using namespace std;

HttpResponseContext::HttpResponseContext(cbFunc cb)
        : WHPSResponseWrapper(cb)
{

}

HttpResponseContext::~HttpResponseContext()
{

}

WhpsWriter& HttpResponseContext::getWriter()
{
        return this->getResponse()._getWriter();
}

HttpResponseContext& HttpResponseContext::getResponse()
{
        return (HttpResponseContext&)*this->_getResponse();
}

void HttpResponseContext::setContentLength(const string& length)
{
        this->getResponse()._setContentLength(length);
}

void HttpResponseContext::setContentType(const string& type)
{
        this->getResponse()._setContentType(type);
}

void HttpResponseContext::addHeader(const string& h_key, const string& h_value)
{
        this->getResponse()._addHeader(h_key, h_value);
}

void HttpResponseContext::addIntHeader(const string& h_key, const int& h_value)
{
        this->getResponse()._addIntHeader(h_key, h_value);
}

void HttpResponseContext::addDateHeader(const string& h_key, const long& h_value)
{
        this->getResponse()._addDateHeader(h_key, h_value);
}

void HttpResponseContext::setHeader(const string& h_key, const string& h_value)
{
        this->getResponse()._setHeader(h_key, h_value);
}

void HttpResponseContext::setIntHeader(const string& h_key, const int& h_value)
{
        this->getResponse()._setIntHeader(h_key, h_value);
}

void HttpResponseContext::setDateHeader(const string& h_key, const long& h_value)
{
        this->getResponse()._setDateHeader(h_key, h_value);
}

string HttpResponseContext::getHeader()
{
        return this->getResponse()._getHeader();
}
