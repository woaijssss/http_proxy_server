
#include <sstream>
#include <fstream>
#include <sys/stat.h>   // for fstat

#include "HelloWhps.h"

#include <iostream>
using namespace std;


static size_t getFileTotalSize(const ifstream& in)
{
        struct stat stat__;

        /* std::filebuf: 流缓冲区用于读取和写入文件。 */
        auto getFd = [](std::filebuf& fbuff) -> int
        {
                // 继承自std::filebuf，才能使用_M_file protected成员
                struct Helper : public std::filebuf
                {
                        int Handle()
                        { return _M_file.fd(); }
                };
            
                return static_cast<Helper&>(fbuff).Handle();
        };

        int fd = getFd(*in.rdbuf());
        fstat(fd, &stat__);

        return stat__.st_size;
}

/* 测试接口 */
static int load(const string& filename, string& f_buff)
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

        cout << "++++++++++++++++++++++++++++++++++" << endl;
        cout << filename << endl;
        cout << "++++++++++++++++++++++++++++++++++" << endl;
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


#define TestSendMsg(TypeName1, TypeName2) load(TypeName1, TypeName2)         

#if 1
static string TestSend(const string& base, const string& resource)
{
        string test_msg;
        // int res = TestSendMsg("./webResource/html/index.html", test_msg);
        int res = TestSendMsg(base + resource, test_msg);

        if (res < 0)
        {
                string tmp = "404 not found!";
                test_msg = "HTTP/1.1 404 not found \r\n"
                             "Content-Length: " + to_string(tmp.size()) + "\r\n"
                             "\r\n"
                             + tmp;
        }

        return test_msg;

        // tcp_session->close();           // 不确定是否需要服务器来释放连接？（目前测试chrome浏览器，必须由服务器释放）
}   
#endif

HelloWhps::HelloWhps()
{

}

HelloWhps::~HelloWhps()
{

}

#include "WebSourceConfig.h"
/* 静态资源自动加载功能（通过配置，获取静态资源路径，并写回给前端） */
static void TestStaticResource(HttpWhpsRequest request, HttpWhpsResponse response)
{
        // response.setContentType("text/html;charset=UTF-8");
        string msg = TestSend(WebSourceConfig::GetInstance().get("StaticResource", "rootDir"), request._url);
        
        response.getWriter().write(msg);
}

void HelloWhps::doGet(HttpWhpsRequest request, HttpWhpsResponse response)
{
        cout << "HelloWhps::doGet" << endl;
        TestStaticResource(request, response);
}

void HelloWhps::doPost(HttpWhpsRequest request, HttpWhpsResponse response)
{

}
