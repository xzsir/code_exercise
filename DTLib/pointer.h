#ifndef POINTER_H
#define POINTER_H

#include "Object.h"

namespace QSLib
{
    template<typename T>
    class Pointer : public Object//Pointer为抽象类
    {
    protected:
        T* m_pointer;

    public:
        Pointer(T* p = NULL)
        {

            m_pointer = p;
        }

        T* operator ->()
        {

            return m_pointer;
        }
        T& operator *()
        {
            return *m_pointer;

        }

        const T* operator ->()const//供const对象使用
        {

            return m_pointer;
        }
        const T& operator *()const//供const对象使用
        {
            return *m_pointer;

        }

        bool isNull()const
        {

            return (m_pointer == NULL);
        }
        T* get()const
        {
            return m_pointer;

        }
        //要求析构函数为纯虚函数，但是可以省略，因为Object类的析构函数为纯虚函数
    };


}


#endif // POINTER_H
