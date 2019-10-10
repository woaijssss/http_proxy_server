/*
 * WHPSHttpStatusCode.cpp
 *
 *  Created on: Oct 10, 2019
 *      Author: wenhan
 */


#include "WHPSHttpStatusCode.h"

WHPSHttpStatusCode::WHPSHttpStatusCode()
{

}

WHPSHttpStatusCode::~WHPSHttpStatusCode()
{

}

void WHPSHttpStatusCode::init()
{
        _m_sc_status[200] = "OK";
        _m_sc_status[201] = "Created";
        _m_sc_status[202] = "Accepted";
        _m_sc_status[204] = "No Content";
        _m_sc_status[401] = "Unauthorized";
        _m_sc_status[403] = "Forbbiden";
        _m_sc_status[404] = "Not Found";
        _m_sc_status[405] = "Method Not Allowed";
        _m_sc_status[500] = "Server Error";
        _m_sc_status[502] = "Bad Gateway";
}

const WHPSHttpStatusCode::StrStatus& WHPSHttpStatusCode::getStatus(const int& st)
{
        return _m_sc_status[st];
}
