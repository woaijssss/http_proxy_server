
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
        cout << "--->: " << (_cb_func? "callable" : "not callable") << endl;
        _cb_func(msg);
}
