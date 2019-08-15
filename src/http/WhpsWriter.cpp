
#include <iostream>
#include "WhpsWriter.h"

using namespace std;

WhpsWriter::WhpsWriter(cbFunc cb)
        : _cb_func(cb)
{

}

WhpsWriter::~WhpsWriter()
{
        
}

void WhpsWriter::write(const string& msg)
{
        if (_cb_func)
        {
                _cb_func(msg);
        }
}
