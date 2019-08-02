
#include "WHPSResponseWrapper.h"

using namespace std;

WHPSResponseWrapper::WHPSResponseWrapper(cbFunc cb)
        : _version("HTTP/1.1")
        , _st_code("200")
        , _status("OK")
        , _writer(cb)
{
        this->initParams();
}

WHPSResponseWrapper::~WHPSResponseWrapper()
{

}

void WHPSResponseWrapper::initParams()
{
        _header["Content-Type"] = "text/html;charset=UTF-8";
}

WHPSResponseWrapper* WHPSResponseWrapper::_getResponse()
{
        return this;
}

void WHPSResponseWrapper::_setContentLength(const std::string& length)
{
        _header["Content-Length"] = length;
}

void WHPSResponseWrapper::_setContentType(const std::string& type)
{
        _header["Content-Type"] = type;
}

void WHPSResponseWrapper::_addHeader(const std::string& h_key, const std::string& h_value)
{
        _header[h_key] = h_value;
}

void WHPSResponseWrapper::_addIntHeader(const std::string& h_key, const int& h_value)
{
        _header[h_key] = to_string(h_value);
}

void WHPSResponseWrapper::_addDateHeader(const std::string& h_key, const long& h_value)
{

}

void WHPSResponseWrapper::_setHeader(const std::string& h_key, const std::string& h_value)
{
        _header[h_key] = h_value;
}

void WHPSResponseWrapper::_setIntHeader(const std::string& h_key, const int& h_value)
{
        _header[h_key] = to_string(h_value);
}

void WHPSResponseWrapper::_setDateHeader(const std::string& h_key, const long& h_value)
{

}

string WHPSResponseWrapper::_getHeader()
{
        string header;
        for (auto& kv: _header)
        {
                header += kv.first + ": " + kv.second + "\r\n";
        }

        return header;
}

WhpsWriter& WHPSResponseWrapper::_getWriter()
{
        return _writer;
}
