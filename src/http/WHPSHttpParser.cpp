
#include <iostream>
#include <string.h>
#include <sstream>      // for std::istringstream
#include <iterator>     // for std::istream_iterator

#include "WHPSImplMacroBase.h"
#include "WHPSHttpParser.h"
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

static void GetKvSplit(HttpHeader__& header, const string& sstr, const string& sep)
{
        String s__(sstr);
        WHPSHttpParser::SpVector spv = s__.splitOnce(":");

        // if (spv.size() == 2)
        {
                header[spv[0]] = spv[1];
        }
}

// 提取请求行中，第一行Url的内容
Impl_GET_CONTENT(Url, WHPSHttpParser::SpVector)

// 根据":"拆分成header的key和value
Impl_GET_KV_SPLIT(":", HttpHeader__)

WHPSHttpParser::WHPSHttpParser()
        : _crlf_old("\n")
        , _crlf("\r\n")
{

}

WHPSHttpParser::~WHPSHttpParser()
{

}

#include "WHPSLog.h"
void WHPSHttpParser::parseHttpRequest(std::string& raw_msg)
{
        HttpRequestContext context;
        WHPSLogDebug(NULL, raw_msg.c_str());

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
        // this->getBodyInfo();
}

bool WHPSHttpParser::getFirstLine(SpVector& vrow_seq, HttpRequestContext& context)
{
        String first_line = String(vrow_seq[0]);
        SpVector req_line = GetContent(first_line.str());
        
        if (req_line.size() != 3)
        {
                return false;
        }

        context._method = req_line[0];
        context._url = req_line[1];
        context._version = req_line[2];

        cout << "method : " << context._method  << endl;
        cout << "url    : " << context._url << endl;
        cout << "version: " << context._version << endl;

        vrow_seq.erase(vrow_seq.begin());       // 处理之后，需要将第一行删除

        return true;
}

void WHPSHttpParser::getHeaderInfo(SpVector& vrow_seq, HttpRequestContext& context)
{
        for (auto& seq_line: vrow_seq)  // 遍历行序列
        {
                if (seq_line == "\0")   // 可能出现空行，调过解析
                {
                        continue;
                }

                cout << "size: " << seq_line.size() << endl;
                /* 将每一行，按照指定符号拆分成map k-v键值对形式 */
                // cout << seq_line << endl;
                GetSplit(context._header, seq_line);
        }
}
