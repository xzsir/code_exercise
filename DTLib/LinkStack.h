#ifndef LINKSTACK_H
#define LINKSTACK_H

#include "Stack.h"
#include "LinkList.h"
#include "exception.h"
namespace QSLib {


template< typename T>
class LinkStack : public Stack<T>
{
protected:
    LinkList<T> m_list;

public:

    void push(const T& obj)
    {
        m_list.insert(0,obj);
    }
    void pop()
    {
        if(m_list.length() > 0)
            m_list.remove(0);
        else
            THROW_EXCEPTION(InvalidOperationException, "mo element to pop...");
    }

    int size()const//const对象调用
    {
        return m_list.length();
    }

    T top()const//const 对象调用
    {
        if(m_list.length() > 0)
            return m_list.get(0);
        else
            THROW_EXCEPTION(InvalidOperationException, "mo element to top...");
    }


    void clear()
    {
        m_list.clear();
    }

};
}





#endif // LINKSTACK_H
