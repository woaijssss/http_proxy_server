
#ifndef __WHPS_WRITER_H__
#define __WHPS_WRITER_H__

#include <string>
#include <functional>

/* http响应回写类
 * 作为HttpResponse的子对象，负责将响应信息，回写给客户端
 */
class WhpsWriter
{
        using cbFunc = std::function<void(const std::string&)>;
public:
        WhpsWriter(cbFunc cb);
        ~WhpsWriter();

public:
        void write(const std::string& msg);

private:
        cbFunc _cb_func;
};

#endif  // __WHPS_WRITER_H__
