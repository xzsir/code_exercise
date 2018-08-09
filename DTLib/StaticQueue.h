#ifndef STATICQUEUE_H
#define STATICQUEUE_H
#include "Queue.h"
#include "Exception.h"

namespace QSLib {

template <typename T, int N>
class StaticQueue : public Queue<T>
{
protected:
    T m_space[N];
    int m_rear;
    int m_front;
    int m_length;

public:
    StaticQueue()
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }

    int capacity()
    {
        return N;
    }
    void add(const T& obj)//使用循环计数法可以使对头队尾不固定，提高效率
    {
        if(m_length < N)
        {
            m_space[m_rear] = obj;
            m_rear = (m_rear + 1) % N;//循环计数法
            m_length++;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"no space to add...");
        }

    }
    void remove()//每次出队列后，原先的元素之后那个元素就作为新的队头
    {
        if(m_length > 0)
        {
            m_front = (m_front + 1) % N;//循环计数法
            m_length--;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"no element to remove...");
        }
    }


    T front() const
    {
        if(m_length > 0)
        {
            return m_space[m_front];
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"no memory to front...");
        }

    }
    void clear()
    {
        m_front = 0;
        m_rear = 0;
        m_length = 0;
    }
    int length() const
    {
        return m_length;
    }

};

}

#endif // STATICQUEUE_H
