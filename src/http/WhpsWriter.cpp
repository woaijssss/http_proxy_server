
#include <iostream>
#include "WhpsWriter.h"
#include "WHPSResponseWrapper.h"

using namespace std;

WhpsWriter::WhpsWriter(WHPSResponseWrapper* resp, cbFunc cb)
        : _resp(resp)
        , _cb_func(cb)
{

}

WhpsWriter::~WhpsWriter()
{
        
}

void WhpsWriter::write(const string& msg)
{
        if (_cb_func)
        {
                if (_resp)
                {
                        _resp->_setIntHeader("Content-Length", msg.size());
                        string sendMsg;
                        sendMsg += _resp->_getHeader();
                        sendMsg += msg;

                        _cb_func(sendMsg);
                }
                else
                {

                }
        }
}
