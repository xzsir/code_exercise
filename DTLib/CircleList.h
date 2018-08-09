#ifndef CIRCLELIST_H
#define CIRCLELIST_H
#include "LinkList.h"

namespace QSLib {

template <typename T>
class CircleList : public LinkList<T>
{
  protected:
    typedef typename LinkList<T>::Node Node;

    int mod(int i)const//O(1)
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
    }
    Node* last()const//O(n)
    {
        return this->position(this->m_length - 1)->next;//倒数第二个的下一个就是最后一个
    }
    void last_to_first()const//O(n)
    {
        last()->next = this->m_header.next;
    }

public:
    bool insert(const T& e)//O(n)
    {
        return insert(this->m_length,e);
    }

    bool insert(int i, const T& e)//O(n)
    {
        bool ret = true;
        i = i % (this->m_length + 1);//计算出需要插入的位置
        ret= LinkList<T> :: insert(i,e);

        if(ret && (i == 0))
        {

            last_to_first();
        }


        return ret;
    }

    bool remove(int i)//O(n)
    {
        bool ret = true;
        i = mod(i);

        if(i == 0)
        {
            Node* toDel = this->m_header.next;//O(1)
            if(toDel != NULL)
            {
                this->m_header.next = toDel->next;
                this->m_length--;
                if(this->m_length > 0)//如果还能构成环
                {
                    last_to_first();
                    if(this->m_current == toDel)
                        this->m_current = toDel->next;
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }
                this->destroy(toDel);//异常安全
            }
            else
            {
                ret = false;
            }
        }
        else
        {
           ret = LinkList<T>::remove(i);
        }
        return ret;
    }



    bool set(int i,const T& e)//O(n)
    {
        return LinkList<T> :: set(mod(i),e);
    }
    T get(int i)const//O(n)
    {
        return LinkList<T>::get(mod(i));
    }
    bool get(int i, T& e)const//此处实现和视频不同，视频在T&前加了const
    {
        return LinkList<T> :: get(mod(i),e);
    }

    /*
    *一开始提到的利用LinkList的find函数查找，先last指向NULL，再find，最后last_to_first，这样是不可靠的，当find里的比较操作符
    * 重载时抛出异常后，就会返回异常，最后链表状态也被改变了，成为了一个单链表
    */
    int find(const T& e)const//O(n)
    {
        int ret = -1;//没找着
        Node* slider = this->m_header.next;
        for(int i = 0; i < this->m_length; i++)
        {
            if(slider->value == e)
            {
                ret = i;
                break;
            }
            slider = slider->next;
        }

        return ret;
    }

    /*
    *不能使用先将首节点指向NULL再LinkList<T>:: clear()
    * 当clear()抛出异常后就改变了链表状态
    *
    */
    void clear()//O(n)
    {

        while(this->m_length > 1)//O(n)
        {
            remove(1);//remove(1)的好处是避免了使用remove(0)从而执行大量语句，拉低效率 //O(1)
        }
        if(this->m_length == 1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step)//O(1)
    {
       return  LinkList<T> :: move(mod(i),step);
    }

    bool end()//O(1)
    {
        return (this->m_length == 0) || (this->m_current == NULL);
    }
    ~CircleList()//O(n)
    {
        clear();
    }

};

}


#endif // CIRCLELIST_H
