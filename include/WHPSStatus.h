/*
 * WHPSStatus.h
 *
 *  Created on: Oct 8, 2019
 *      Author: wenhan
 */

#ifndef __WHPSSTATUS_H__
#define __WHPSSTATUS_H__

#include <string>

/* 状态器，提供各种状态的通用接口。
 * 实现具体的状态管理，需要继承该类，并实现接口。
 */
class WHPSStatus
{
public:
        using Status_t = int;
        using StrStatus = std::string;
public:
        virtual ~WHPSStatus()
        {

        }

public:
        /* 初始化状态器参数 */
        virtual void init() = 0;

        /* 获取状态
         * 直接返回默认状态
         */
        virtual const Status_t& getStatus()
        {
                return _itp;
        }

        /* 获取状态
         *      状态的保存为 k(int)-v(string) 形式
         */
        virtual const StrStatus& getStatus(const int& st)
        {
                return _stp;
        }

        /* 获取状态
         *      状态的保存为 k(string)-v(string) 形式
         */
        virtual const StrStatus& getStatus(const std::string& st)
        {
                return _stp;
        }

protected:
        WHPSStatus() {}

private:
        WHPSStatus(WHPSStatus& st) = delete;

        Status_t  _itp;
        StrStatus _stp;
};

#endif /* __WHPSSTATUS_H__ */
