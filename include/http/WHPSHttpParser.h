
#ifndef __WHPS_HTTP_PARSER_H__
#define __WHPS_HTTP_PARSER_H__

#include <map>

#include "String.h"
#include "WHPSRequest.h"
#include "WHPSResponse.h"

/* HTTP请求报文解析
 *  主要功能：
 *      （1）解析http请求url
 *      （2）解析http请求头
 *      （3）解析http请求body（GET和DELETE时，body应为空）
 *      （4）将解析后的资源信息、请求头、请求体合成一个大的结构体，返回给外部调用
 */
/* class WHPSHttpParser; */

class WHPSHttpParser
{
public:
        using SpVector = String::SVector;       // SplitVector

        WHPSHttpParser();
        ~WHPSHttpParser();

public:
        /* 从原始数据解析出http各项参数 */
        void parseHttpRequest(std::string& raw_msg, HttpRequestContext& context);

private:        
        /* 以下为各部分的解析方法，参数必须包含行序列和请求内容结构，
         * 每一步骤，自行补全"请求内容"结构中的相关值
         */
        /* 解析的第一步：
         *      从原始数据，按每一行拆分出行序列，并按行提取数据
         */
        void parseSequence(SpVector& vrow_seq, HttpRequestContext& context);

        /* 第一行(请求行)要特殊处理 */
        bool getFirstLine(SpVector& vrow_seq, HttpRequestContext& context);

        /* 提取第一行请求后，根据url格式来判断请求的是否是静态资源；
         * 如果是静态资源，无需调用至用户应用层，在系统层直接返回资源文件；
         * 如果不是静态资源，则需要调用至用户层，并通过用户开发的应用程序返回相应的资源。
         */
        bool checkResourceType(const std::string& url);

        /* 获取http请求头信息 */
        void getHeaderInfo(SpVector& vrow_seq, HttpRequestContext& context);

        /* 获取http请求体信息 */
        bool getBodyInfo(SpVector& vrow_seq, HttpRequestContext& context);

private:
        const char* _crlf_old/* = "\n"*/;   // 兼容老版本或不完整的http，提高兼容性(详见协议)
        const char* _crlf/* = "\r\n"*/;   // 标准http请求行，都是以"\r\n"结尾分割
};

#endif  // __WHPS_HTTP_PARSER_H__