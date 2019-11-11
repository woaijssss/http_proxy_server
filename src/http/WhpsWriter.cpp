#include <iostream>
#include "WhpsWriter.h"
#include "WHPSResponseWrapper.h"

using namespace std;

WhpsWriter::WhpsWriter(WHPSResponseWrapper* resp, cbFunc cb)
        : m_resp(resp),
          m_cbFunc(cb)
{

}

WhpsWriter::~WhpsWriter()
{

}

void WhpsWriter::write(const string& msg)
{
        if (m_cbFunc)
        {
                if (m_resp)
                {
                        m_resp->_setIntHeader("Content-Length", msg.size());
                        string sendMsg;
                        sendMsg += m_resp->_getHeader();
                        sendMsg += msg;

                        m_cbFunc(sendMsg);
                } else
                {

                }
        }
}
