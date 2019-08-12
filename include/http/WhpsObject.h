
#ifndef _WHPS_OBJECT_H__
#define _WHPS_OBJECT_H__

#include <typeinfo>
#include <memory>

#include "WhpsObjRegisterBase.h"
#include "HttpWhpsFactory.h"

template<class T>
class WhpsObject : public WhpsObjRegisterBase, public HttpWhps
{
public:
        // static WhpsObjRegisterBase * CreateObject()
        static std::shared_ptr<HttpWhps> CreateObject()
        {
                // return new T();
                std::shared_ptr<HttpWhps> ptr(new T());
                return ptr;
        }
 
        struct Aux
        {
                Aux()
                {
                        if (!GetHttpWhpsFactory()->regist(typeid(T).name(), CreateObject))
                        {
                                cout << "regist failed" << endl;
                        }
                        else
                        {
                                cout << "regist success" << endl;
                        }
                }

                inline void do_nothing()const { }
        };
 
        static Aux _aux;        // 类名注册辅助器
 
public:
        WhpsObject()
        {
             
        }
 
        virtual ~WhpsObject()
        {
                _aux.do_nothing();
        }
};

template <class T>
typename WhpsObject<T>::Aux WhpsObject<T>::_aux;

#endif  // _WHPS_OBJECT_H__