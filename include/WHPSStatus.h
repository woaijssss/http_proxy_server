/*
 * WHPSStatus.h
 *
 *  Created on: Oct 8, 2019
 *      Author: wenhan
 */

#ifndef __WHPSSTATUS_H__
#define __WHPSSTATUS_H__

/* 状态器，提供各种状态的通用接口。
 * 实现具体的状态管理，需要继承该类，并实现接口。
 */
class WHPSStatus
{
        using status_t = int;
public:
        WHPSStatus();

        virtual ~WHPSStatus();

public:
        /* 初始化状态器参数 */
        virtual void init() = 0;

        /* 获取状态
         * 直接返回默认状态
         */
        virtual const status_t& getStatus() const = 0;

        /* 获取状态
         *      状态的保存为 k(int)-v 形式
         */
        virtual const status_t& getStatus(const int& k) const = 0;

        /* 获取状态
         *      状态的保存为 k(string)-v 形式
         */
        virtual const status_t& getStatus() const = 0;
};

#endif /* __WHPSSTATUS_H__ */
