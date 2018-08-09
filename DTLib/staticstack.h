#ifndef STATICSTACK_H
#define STATICSTACK_H

#include "Stack.h"
#include "LinkList.h"
#include "Exception.h"
#include <iostream>

using namespace std;


namespace QSLib {

template <typename T,int N>
class StaticStack : public Stack<T>
{
protected:
    T m_space[N];
    int m_top;
    int m_size;
    LinkList<T> m_min;
    LinkList<T> m_max;

public:
    StaticStack()
    {
        m_top = -1;//为什么?因为没有数据元素时没有栈顶
        m_size = 0;

    }
    int capacity()
    {
        return N;
    }

    void push(const T& obj)
    {
        if(m_size < N)
        {
            m_space[m_top + 1] = obj;//保持异常安全，当e为类对象时可能失败，此时要是不成功发生异常返回后对栈本身并没有影响
            m_top++;
            m_size++;
            if(m_top == 0)//将栈的第一个数据作为最值
            {

               m_min.insert(obj);
               m_max.insert(obj);
            }
            else
            {
                T getmin = m_min.get(0);
                //在最小值处理中，若比上一个最小值小，将数据插入到0位置，否则插入到最后，这样插入后链表的数据顺序就是从小到大
                if(getmin > obj)
                {
                    m_min.insert(0,obj);
                }

                else if(getmin < obj)
                {
                    m_min.insert(obj);
                }

                //在最大值处理中，若比上一个最大值大，则将数据插入到0位置，否则插入到最后位置，这样插入后链表的数据顺序就是从大到小。
                T getmax = m_max.get(0);
                if(getmax < obj)
                {
                    m_max.insert(0,obj);
                }
                else if(getmax > obj)
                {
                    m_min.insert(obj);
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"no memory to push...");
        }
    }
    void pop()
    {
        if(m_size > 0)
        {

            //如果在出栈时将最大值或者最小值弹出来了怎么办？弹出来后最值又该如何确定
            if(top() == m_min.get(0))
            {
                cout << "top() == m_min.get(0)" << endl;
                m_min.remove(0);//链表的0位置始终保存着最小值
            }
            if(top() == m_max.get(0))
            {
                cout << "top() == m_max.get(0)" << endl;
                m_max.remove(0);//链表的0位置始终保存着最大值
            }

            m_size--;//技巧：处理后再改变栈的状态
            m_top--;

        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"InvalidOperation to pop...");
        }

    }
    int size()const//const对象也能调用
    {
        return m_size;
    }
    T top()const//const 对象调用
    {
        if(m_size > 0)
        {
            return m_space[m_top];
        }
        else
        {
           THROW_EXCEPTION(InvalidOperationException,"InvalidOperation to top...");
        }
    }
    void clear()
    {
        m_size = 0;
        m_top = -1;
    }

    //缺陷：只有插入元素各不相同才能用
    T getMax()
    {
       return m_max.get(0);
    }

    T getMin()
    {
        return m_min.get(0);
    }

    //最后需不需要析构函数？
    //

};

}


#endif // STATICSTACK_H
