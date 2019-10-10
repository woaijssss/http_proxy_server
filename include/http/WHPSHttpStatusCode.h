/*
 * WHPSHttpStatusCode.h
 *
 *  Created on: Oct 8, 2019
 *      Author: wenhan
 */

#ifndef __WHPSHTTPSTATUSCODE_H__
#define __WHPSHTTPSTATUSCODE_H__

#include <map>

#include "WHPSStatus.h"

class WHPSHttpStatusCode final: public WHPSStatus
{
public:
        WHPSHttpStatusCode();

        virtual ~WHPSHttpStatusCode();

public:
        /* 初始化状态器参数 */
        virtual void init();

        /* 获取状态
         *      状态的保存为 k(int)-v(string) 形式
         */
        virtual const StrStatus& getStatus(const int& st);

private:
        std::map<int, std::string> _m_sc_status;        // 保存常用状态码与状态信息的对应关系(后面加到配置文件中)
};

#endif /* __WHPSHTTPSTATUSCODE_H__ */
