
#ifndef __IMPL_SINGLETON_H__
#define __IMPL_SINGLETON_H__

#include <iostream>
using namespace std;

/* 单例虚类
 * 将GC功能放在虚类中，子类无须考虑单例的释放问题，仅需继承该类即可。
 */
template<class C_T>
class ImplSingleton
{
public:
        virtual ~ImplSingleton() {}

protected:
        ImplSingleton() {}

        static void free()
        {
                _gc.free();
        }

private:
        class GC            // 避免内存泄漏的垃圾回收(嵌套)类
        {
        public:
                // ~GC()
                void free()
                {
                        if (C_T::Get())
                        {
                                cout << "-----delete" << endl;
                                delete C_T::Get();
                        }
                }
        };
        static GC _gc; 
};

template<class C_T>
typename ImplSingleton<C_T>::GC ImplSingleton<C_T>::_gc;

#endif  // __IMPL_SINGLETON_H__