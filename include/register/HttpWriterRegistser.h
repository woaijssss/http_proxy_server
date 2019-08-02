/*
 * HttpWriterRegistser.h
 *
 *  Created on: Aug 2, 2019
 *      Author: wenhan
 */

#ifndef __HTTP_WRITER_REGISTSER_H__
#define __HTTP_WRITER_REGISTSER_H__

#include "ImplRegisterBase.h"

template<class T>
class HttpWriterRegistser: public ImplRegisterBase<T>
{
public:
        using cbFunc = typename ImplRegisterBase<T>::cbFunc;
public:
        HttpWriterRegistser()
                : ImplRegisterBase<T>()
        {

        }
        virtual ~HttpWriterRegistser()
        {

        }

public:
        /* 注册http回写接口 */
        virtual void registObj(cbFunc cb)
        {
                _cb_func = cb;
        }

        virtual cbFunc getRegistObj()
        {
                return _cb_func;
        }

private:
        cbFunc _cb_func;
};

#endif  //__HTTP_WRITER_REGISTSER_H__
