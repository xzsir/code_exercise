#ifndef ARRAY_H
#define ARRAY_H
#include "Object.h"
#include "Exception.h"


namespace QSLib
{
    template <typename T>
    class Array : public Object
    {
    protected:
        T* m_array;

    public:
        virtual bool set(int i, const T& e)
        {
            bool ret = (i >= 0) && (i < length());

            if(ret)
            {
                m_array[i] = e;
            }
             return ret;
        }
        bool get(int i, T& e)const
        {
            bool ret = (i >= 0) && (i < length());

            if(ret)
            {
                e = m_array[i];
            }
             return ret;
        }


        T& operator [](int i)
        {
            if((i >= 0) && (i < length()))
            {
                return m_array[i];
            }
            else
            {
                THROW_EXCEPTION(IndexOutOfBoundsException,"Parameter i is Out of Bounds...");
            }
        }
        T  operator [](int i)const//当const对象使用[]操作符时首先会将它转换成普通对象，然后再使用(*this)[i]这时就会自动调用非const版本的重载
        {
            return const_cast<Array<T>&>(*this)[i];
        }
        virtual int length()const = 0;
    };

}

#endif // ARRAY_H
