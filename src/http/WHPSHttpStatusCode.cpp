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
        m_scStatus[200] = "OK";
        m_scStatus[201] = "Created";
        m_scStatus[202] = "Accepted";
        m_scStatus[204] = "No Content";
        m_scStatus[401] = "Unauthorized";
        m_scStatus[403] = "Forbbiden";
        m_scStatus[404] = "Not Found";
        m_scStatus[405] = "Method Not Allowed";
        m_scStatus[500] = "Server Error";
        m_scStatus[502] = "Bad Gateway";
}

const WHPSHttpStatusCode::StrStatus& WHPSHttpStatusCode::getStatus(const int& st)
{
        return m_scStatus[st];
}
