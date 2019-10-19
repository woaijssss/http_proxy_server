
#include <iostream>
#include <string.h>
#include <sstream>      // for std::istringstream
#include <iterator>     // for std::istream_iterator

#include "WHPSImplMacroBase.h"
#include "WHPSHttpParser.h"
#include "WHPSLog.h"
#include "util.h"

using namespace std;

/* 针对http请求行第一行，获取：
 *      请求方法、url和http版本信息
 */
static WHPSHttpParser::SpVector GetUrl(const string& sstr)
{
        stringstream iss(sstr);
        WHPSHttpParser::SpVector req_line;
        std::copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(req_line));

        return req_line;
}

static void GetKvSplit(HttpRequestContext& request, const string& sstr, const string& sep)
{
        if (sstr.empty())
        {
                return;
        }

        String s__(sstr);
        WHPSHttpParser::SpVector spv = s__.splitOnce(":");

        if (spv.size() == 2)         // 最好加上，双层判断保险
        {
                // header[spv[0]] = spv[1];
                request.setHeader(spv[0], spv[1]);
        }
}

/* 获取http请求体
 *      当前是按照文本保存，一般是json或text
 *      但若要支持文件上传等功能，可能需要做二进制数据的缓存（后续扩展）
 */
static bool GetContactContent(HttpRequestContext& request, WHPSHttpParser::SpVector& vrow_seq, const char* crlf)
{
        if (vrow_seq.empty())
        {
                return false;
        }

#if 0
        for (string s: vrow_seq)
        {
                body += (s+crlf);
        }
#else
        /* 标准http的body里面没有"\r\n"的符号，只有结尾有，
         * body的每一行应该是 '\n' 结尾。
         * 而vrow_seq序列在处理请求行的时候，已经将读取完的数据在序列中清除，
         * 因此，获取body时，不需要再进行拼接了，序列中下标为0的元素，就是body。
         */
        // body = vrow_seq[0];
        request.setBody(vrow_seq[0]);
#endif

        return true;
}

// 提取请求行中，第一行Url的内容
Impl_GET_CONTENT(Url, WHPSHttpParser::SpVector)

// 根据":"拆分成header的key和value
Impl_GET_KV_SPLIT(HttpRequestContext, ":")

Impl_GET_CONTACT(HttpRequestContext, WHPSHttpParser::SpVector, "\r\n")

WHPSHttpParser::WHPSHttpParser()
        : _crlf_old("\n")
        , _crlf("\r\n")
{

}

WHPSHttpParser::~WHPSHttpParser()
{

}

void WHPSHttpParser::parseHttpRequest(std::string& raw_msg, HttpRequestContext& context)
{
#if 0   // 用于调试http输出的原始字符
        for (int i = 0; i < raw_msg.size(); i++)
        {
                printf("(%d, %c)", raw_msg[i], raw_msg[i]);
        }
        cout << endl;
#endif

        // 当前没有做包完整性校验，测试阶段默认一次请求一个完整的包
        SpVector vrow_seq = String(raw_msg).split(_crlf);
        this->parseSequence(vrow_seq, context);
}

/*
        struct HttpRequestContext
        {
                std::string                             _method;        // 请求方法
                std::string                             _url;           // 请求资源路径
                std::string                             _version;       // 协议版本
                std::map<std::string, std::string>      _header;        // 请求头(K-V格式)
                std::string                             _body;          // 请求体
        };
 */
void WHPSHttpParser::parseSequence(SpVector& vrow_seq, HttpRequestContext& context)
{
        if (!vrow_seq.size())   // 如果行序列为空，则直接返回
        {
                return;
        }

        if (!this->getFirstLine(vrow_seq, context))     // http请求第一行(特殊处理)
        {
                return;
        }

        this->getHeaderInfo(vrow_seq, context);         // http请求header信息
        
        if (!this->getBodyInfo(vrow_seq, context))      // http请求body信息
        {
                return;
        }
}

bool WHPSHttpParser::getFirstLine(SpVector& vrow_seq, HttpRequestContext& context)
{
        String first_line = String(vrow_seq[0]);
        SpVector req_line = GetContent(first_line.str());
        
        /* 格式检查：
         *      必须符合： METHOD URL VERSION 格式
         *      否则返回失败
         */
        if (req_line.size() != 3)
        {
                return false;
        }

        const string& method = req_line[0];
        const string& url = String(req_line[1]).decode("UrlCode");
        context.setMethod(method);
        context.setUrl(url);	// 防止中文
        context.setUrlParams(url);
        context.setVersion(req_line[2]);
        context.setFlag(this->checkResourceType(context));  // 判断资源类型
        vrow_seq.erase(vrow_seq.begin());       // 处理之后，需要将第一行删除

        /* 当前仅可支持
         *      GET
         *      POST
         *      PUT
         *      DELETE
         * 四种方法 
        */
        if (method != "GET"
                && method != "POST"
                && method != "PUT"
                && method != "DELETE")
        {
                WHPSLogWarn("not support " + method);

                return false;
        }

        return true;
}

bool WHPSHttpParser::checkResourceType(HttpRequestContext& request)
{
        /* （1）检测的关键是 url 中是否带有相应的关键字符（比如"?"、"&"、"="等字符）
         * （2）静态资源格式必须符合：
         *          http://192.168.12.250:3000/A/B/C/D/1.png
         *      对应的 url 应为：
         *          /A/B/C/D/1.png
         *      <注意>：url不能包含"?"、"&"、"="等任何与参数相关的字符
         * （3）若上述格式全部满足，仍然找不到文件，则报错并返回404
         */
        String sUrl(request.getUrl());

        /* 按照http协议
         *  仅 GET 方法
         *  带参数分隔
         *  带参数列表
         *  带参数赋值判断
         * 均不是静态资源。
         */
        if (request.getMethod() == "GET" && (!sUrl.matchCase("?") && !sUrl.matchCase("&") && !sUrl.matchCase("=")))
        {
                return true;
        }

        return false;
}

void WHPSHttpParser::getHeaderInfo(SpVector& vrow_seq, HttpRequestContext& context)
{
        int count = -1;

        // for (SpVector::iterator it = vrow_seq.begin(); it != vrow_seq.end(); ++it)
        for (auto& seq_line: vrow_seq)  // 遍历行序列
        {
                count++;
                if (seq_line == "\0")   // 可能出现空行，调过解析
                {
                        continue;
                }

                /* 将每一行，按照指定符号拆分成map k-v键值对形式 */
                GetSplit(context, seq_line);
        }

        vrow_seq.erase(vrow_seq.begin(), vrow_seq.begin()+count);
}

bool WHPSHttpParser::getBodyInfo(SpVector& vrow_seq, HttpRequestContext& context)
{
        /* GET和DELETE方法没有请求消息体
         */
        const string& method = context.getMethod();
        if (method == "GET" || method == "DELETE")
        {
                return true;
        }

        if (!GetContact(context, vrow_seq))
        {
                return false;
        }

        return true;
}
