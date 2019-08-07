
#ifndef __CODEC_H__
#define __CODEC_H__

#include <string>

/* 字符编解码器
 * 提供直接调用的编解码接口
 */
class Codec
{

public:
        /* 字符串编码
         */
        static std::string encode(const std::string& str, const std::string& type = "utf-8");
        
        /* 字符串解码
         */
        static std::string decode(const std::string& str, const std::string& type = "utf-8");

private:
        static std::string urlEncode(const std::string& str);
        
        static std::string urlDecode(const std::string& str);
};

#endif  // __CODEC_H__