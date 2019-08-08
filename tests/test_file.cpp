
#include <iostream>
#include <string>  
#include <fstream>  
#include <sstream>

using namespace std;

void load(const string& filename, string& f_buff)
{
#if 0
        std::ifstream t(filename);  
        std::stringstream buffer;  
        buffer << t.rdbuf();  

        f_buff = buffer.str();
#else
        std::ifstream t;  
        int length;  
        t.open(filename, std::ios::binary);                       // open input file  
        t.seekg(0, std::ios::end);              // go to the end  
        length = t.tellg();                     // report location (this is the length)  
        t.seekg(0, std::ios::beg);              // go back to the beginning  
        char *buffer = new char[length];        // allocate memory for a buffer of appropriate dimension  
        t.read(buffer, length);                 // read the whole file into the buffer  
        t.close();                              // close file handle
        f_buff = string(buffer);
        delete[] buffer;
#endif
}

int main()
{
        string f_buff;
        load("/home/wenhan/http_proxy_server/webResource/file_test/curl-7.26.0.tar.gz", f_buff);

        cout << f_buff.size() << endl;

        return 0;
}