#ifndef LIST_H
#define LIST_H
#include "Object.h"

namespace QSLib
{
    template <typename T>
    class List : public Object//线性表可能被当做数组误用，所以需要开发数组类来代替原生数组
    {
    protected:
        List(const List& );
        List& operator =(const List&);//禁止拷贝构造和赋值
    public:
        List(){}
        //不希望被改变时用const
        virtual bool insert(int i,const T& e) = 0;
        virtual bool insert(const T& e) = 0;
        virtual bool remove(int i) = 0;
        virtual bool set(int i,const T& e) = 0;
        virtual bool get(int i,T& e )const = 0;
        virtual int  find(const T& e )const = 0;//实参不希望被改变，也不希望改变成员变量
        virtual int  length()const = 0;
        virtual void clear() = 0;
    };
}

#endif
