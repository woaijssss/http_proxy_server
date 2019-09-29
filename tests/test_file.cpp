
#include <iostream>
#include <string>  
#include <fstream>  
#include <sstream>
#include <sys/stat.h>

using namespace std;

#if 0
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
#else
/* 获取文件大小 */
static size_t getFileTotalSize(const ifstream& in)
{
/*
        struct stat {
               dev_t     st_dev;     // ID of device containing file 
               ino_t     st_ino;     // inode number 
               mode_t    st_mode;    // protection 
               nlink_t   st_nlink;   // number of hard links 
               uid_t     st_uid;     // user ID of owner 
               gid_t     st_gid;     // group ID of owner 
               dev_t     st_rdev;    // device ID (if special file) 
               off_t     st_size;    // total size, in bytes 
               blksize_t st_blksize; // blocksize for file system I/O 
               blkcnt_t  st_blocks;  // number of 512B blocks allocated 
               time_t    st_atime;   // time of last access 
               time_t    st_mtime;   // time of last modification 
               time_t    st_ctime;   // time of last status change 
        };
 */
        struct stat stat__;

        /* std::filebuf: 流缓冲区用于读取和写入文件。 */
        auto getFd = [](std::filebuf& fbuff) -> int
        {
                // 继承自std::filebuf，才能使用_M_file protected成员
                struct Helper : public std::filebuf
                {
                        int Handle()
                        {
                                return _M_file.fd();
                        }
                };

                return static_cast<Helper&>(fbuff).Handle();
        };

        int fd = getFd(*in.rdbuf());
        fstat(fd, &stat__);

        return stat__.st_size;
}

int load(const string& filename, string& f_buff)
{
        std::ifstream in;
        in.open(filename, std::ios::in | std::ios::binary);      // open input file

        if (!in)
        {
                return -1;
        }


        size_t length = getFileTotalSize(in);

        char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
        in.read(buffer, length);       // read the whole file into the buffer  
        in.close();

#if 1
        f_buff = "HTTP/1.1 200 OK \r\n"
//                 "Connection:close \r\n"
                // "Content-Type:application/x-gzip\r\n"
//                  "Content-Type:text/html\r\n"
                 "Content-Length: " + to_string(length) + "\r\n"
                 "\r\n";
#endif
        f_buff += string(buffer, length);
        delete[] buffer;

        return 0;
}
#endif

int main()
{
	while (true)
{
        string f_buff;
        load("/home/wenhan/http_proxy_server/webResource/file_test/curl-7.26.0.tar.gz", f_buff);

        cout << f_buff.size() << endl;
}
        return 0;
}
