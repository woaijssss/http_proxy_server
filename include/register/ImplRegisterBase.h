/*
 * ImplRegisterBase.h
 *
 *  Created on: Aug 2, 2019
 *      Author: wenhan
 */

#ifndef __IMPL_REGISTER_BASE_H__
#define __IMPL_REGISTER_BASE_H__

#include <functional>
#include <string>

/* 功能注册器接口 */
template<class T>
class ImplRegisterBase
{
public:
        using cbFunc = std::function<void(const std::string&)>;
public:
        virtual ~ImplRegisterBase()
        {

        }

protected:
        ImplRegisterBase()
        {

        }

protected:      // 接口集
        /* 注册接口入口 */
        virtual void registObj(cbFunc cb) = 0;

        /* 获取注册的接口 */
        virtual cbFunc getRegistObj() = 0;
};

#endif  // __IMPL_REGISTER_BASE_H__
