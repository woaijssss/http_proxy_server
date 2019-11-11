#include "WHPSResponseWrapper.h"

using namespace std;

WHPSResponseWrapper::WHPSResponseWrapper(cbFunc cb)
        : m_version("HTTP/1.1"),
          m_stCode("200"),
          m_status("OK"),
          m_writer(this, cb)
{
        this->initParams();
}

WHPSResponseWrapper::~WHPSResponseWrapper()
{

}

void WHPSResponseWrapper::initParams()
{
//        m_header["Content-Type"] = "text/html;charset=UTF-8";
}

WHPSResponseWrapper* WHPSResponseWrapper::_getResponse()
{
        return this;
}

void WHPSResponseWrapper::_setContentLength(const std::string& length)
{
        m_header["Content-Length"] = length;
}

void WHPSResponseWrapper::_setContentType(const std::string& type)
{
        m_header["Content-Type"] = type;
}

void WHPSResponseWrapper::_setStatus(const int& sc)
{
        m_stCode = to_string(sc);
        m_status = m_statusMachine.getStatus(sc);
}

void WHPSResponseWrapper::_setError(const int& sc, const std::string& msg)
{
        m_stCode = to_string(sc);
        m_status = msg;
}

void WHPSResponseWrapper::_addHeader(const std::string& h_key, const std::string& h_value)
{
        m_header[h_key] = h_value;
}

void WHPSResponseWrapper::_addIntHeader(const std::string& h_key, const int& h_value)
{
        m_header[h_key] = to_string(h_value);
}

void WHPSResponseWrapper::_addDateHeader(const std::string& h_key, const long& h_value)
{

}

void WHPSResponseWrapper::_setHeader(const std::string& h_key, const std::string& h_value)
{
        m_header[h_key] = h_value;
}

void WHPSResponseWrapper::_setIntHeader(const std::string& h_key, const int& h_value)
{
        m_header[h_key] = to_string(h_value);
}

void WHPSResponseWrapper::_setDateHeader(const std::string& h_key, const long& h_value)
{

}

string WHPSResponseWrapper::_getHeader()
{
        string header = "HTTP/1.1 " + m_stCode + " " + m_status + "\r\n";        // 默认先使用 Http 1.1 版本

        for (auto& kv : m_header)
        {
                header += kv.first + ": " + kv.second + "\r\n";
        }

        header += "\r\n";

        return header;
}

WhpsWriter& WHPSResponseWrapper::_getWriter()
{
        return m_writer;
}
