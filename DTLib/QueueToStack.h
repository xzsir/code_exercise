#ifndef QUEUETOSTAKC_H
#define QUEUETOSTAKC_H
#include "Stack.h"
#include "LinkQueue.h"


namespace QSLib {
template <typename T>
class QueueToStack : public Stack<T>
{
protected:
    LinkQueue<T> m_queue1;
    LinkQueue<T> m_queue2;
    LinkQueue<T>* m_qIn;
    LinkQueue<T>* m_qOut;

    void Move_In_To_Out()const
    {
        int len = m_qIn->length() - 1;
        for(int i = 0; i < len; i++)
        {
            m_qOut->add(m_qIn->front());
            m_qIn->remove();
        }
    }

    void Swap_In_Out()
    {
        LinkQueue<T>* temp;
        temp = m_qIn;
        m_qIn = m_qOut;
        m_qOut = temp;
    }

public:

    QueueToStack()
    {
        m_qIn = &m_queue1;
        m_qOut = &m_queue2;
    }

    void push(const T& obj)
    {
        m_qIn->add(obj);
    }
    void pop()
    {
        if(m_qIn->length() > 0)
        {
            Move_In_To_Out();
            m_qIn->remove();
            Swap_In_Out();
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"No element in current stack to pop");
        }
    }
    int size()const
    {
        return m_qIn->length() + m_qOut->length();
    }
    T top()const
    {
        if(m_qIn->length() > 0)
        {
            Move_In_To_Out();
            return m_qIn->front();

        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"No element in current stack to top ");
        }
    }
    void clear()
    {
        m_qIn->clear();
        m_qOut->clear();
    }

};

}

#endif // QUEUETOSTAKC_H
