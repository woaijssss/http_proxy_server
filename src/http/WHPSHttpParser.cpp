
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

Impl_GET_CONTENT(Url, WHPSHttpParser::SpVector)

WHPSHttpParser::WHPSHttpParser()
        : _crlf("\r\n")
{

}

WHPSHttpParser::~WHPSHttpParser()
{

}

void WHPSHttpParser::parseHttpRequest(std::string& raw_msg)
{
        HttpRequestContext context;

        cout << "======" << endl;
        // cout << raw_msg << endl;

        // 当前没有做包完整性校验，测试阶段默认一次请求一个完整的包
        SpVector vrow_seq = String(raw_msg).split(_crlf);
        this->parseSequence(vrow_seq, context);

        cout << "======" << endl;
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

        for (auto& seq_line: vrow_seq)  // 编译行序列
        {
                cout << seq_line << endl;
        }
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

        return true;
}
