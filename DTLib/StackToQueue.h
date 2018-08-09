#ifndef STACKTOQUEUE_H
#define STACKTOQUEUE_H

#include "LinkStack.h"
#include "Queue.h"

namespace QSLib {

template <typename T>
class StackToQueue : public Queue<T>
{
protected:
    mutable LinkStack<T> m_stack_in;//用于压入数据
    mutable LinkStack<T> m_stack_out;//用于弹出数据
    void move_in_to_out() const  //const只能调用const
    {
        if(m_stack_out.size() == 0)
        {
            while(m_stack_in.size() > 0)
            {
                m_stack_out.push(m_stack_in.top());
                m_stack_in.pop();
            }
        }
    }
public:
    void add(const T& obj)
    {
        m_stack_in.push(obj);
    }

    void remove()
    {
        move_in_to_out();

        if(m_stack_out.size() > 0)
        {
            m_stack_out.pop();
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"No element in current queue");
        }

    }

    T front() const
    {
        move_in_to_out();

        if(m_stack_out.size() > 0)
        {
            return m_stack_out.top();
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"No element in current queue");
        }


    }
    void clear()
    {
        m_stack_in.clear();
        m_stack_out.clear();
    }
    int length() const
    {
        return  m_stack_out.size() + m_stack_in.size();
    }

};


}

#endif // STACKTOQUEUE_H
