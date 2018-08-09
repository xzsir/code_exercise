#ifndef LINUXQUEUE_H
#define LINUXQUEUE_H
#include "Queue.h"
#include "LinuxList.h"
#include "Exception.h"

namespace QSLib {

template<typename T>
class LinuxQueue : public Queue<T>
{
protected:
    struct Node//双向循环链表只包含指针域，数据域需要自行封装
    {
      list_head head;//包含next和prev
      T value;
    };

    list_head m_header;//定义头节点
    int m_length;

public:
    LinuxQueue()//
    {
        m_length = 0;
        INIT_LIST_HEAD(&m_header);//初始化，建立双向循环链表，头节点也作为双向循环链表中的一部分
    }

    void add(const T& obj)//O(n)
    {
       Node* node = new Node();
       if(node != NULL)
       {
           node->value = obj;
           list_add_tail(&node->head,&m_header);//插入尾部，将node->head插入到m_header之前，即插在末尾位置
           m_length++;
       }
       else
       {
          THROW_EXCEPTION(InvalidOperationException,"no memeory in LinuxQueue to new...");
       }
    }
    void remove()//O(1)
    {
        if(m_length > 0)
        {
            list_head* toDel = m_header.next;
            list_del(toDel);//删除即断开原来的建立新的连接
            m_length--;
            delete list_entry(toDel, Node, head);//指针：toDel  类型：Node   成员：head   作用：获取toDel指针所在结构的首地址，，即结构体地址
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
            return list_entry(m_header.next, Node, head)->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException,"no memory to front...");
        }

    }
    void clear()
    {
        while(m_length > 0)
        {
            remove();
        }
    }
    int length() const
    {
        return m_length;
    }

    ~LinuxQueue()
    {
        clear();
    }
};

}


#endif // LINUXQUEUE_H
