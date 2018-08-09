#ifndef DUALCIRCLELIST_H
#define DUALCIRCLELIST_H

#include "DualLinkList.h"
#include "LinuxList.h"

namespace QSLib {

    template <typename T>
    class DualCircleList : public DualLinkList<T>
    {
      protected:
        struct Node : public Object
        {
            list_head head;
            T value;

        };

        list_head m_header;
        list_head* m_current;
        list_head* position(int i)const
        {
            list_head* ret = const_cast<list_head*>(&m_header);//const成员函数不能直接取成员变量的地址
            for(int p = 0; p < i; p++)
            {
                ret = ret->next;
            }

            return ret;
        }


        int mod(int i)const
        {
            return (this->m_length == 0) ? 0 : (i % this->m_length);
        }

    public:
        DualCircleList()
        {
            this->m_length = 0;
            this->m_step = 0;

            m_current = NULL;
            INIT_LIST_HEAD(&m_header);
        }

        bool insert(const T& e)
        {
            return insert(this->m_length,e);
        }

        bool insert(int i, const T& e)
        {
             Node* node = new Node();
             i = i % (this->m_length + 1);
             bool ret = true;

             if(node != NULL)
             {
                node->value = e;
                list_add_tail(&node->head,position(i)->next);//

                this->m_length++;
             }
             else
             {
                 THROW_EXCEPTION(NoEnoughMemoryException,"No memory to insert new element...");
             }
             return ret;
        }

        bool remove(int i)
        {
            bool ret = true;
            i = mod(i);

            ret = (i >= 0) && (i < this->m_length);

            if(ret)
            {
                list_head* toDel = position(i)->next;

                if(m_current == toDel)
                {
                    m_current = toDel->next;
                }

                list_del(toDel);
                this->m_length--;

                delete list_entry(toDel,Node,head);//不能直接delete toDel，因为toDel并不是Node类型

            }

            return ret;
        }

        bool set(int i,const T& e)
        {
            i = mod(i);

            bool ret = true;

            ret = (i >= 0) && (i < this->m_length);

            if(ret)
            {
                list_entry(position(i)->next, Node, head)->value = e;
            }

            return ret;
        }
        bool get(int i,T& e )const
        {
            bool ret = true;
            i = mod(i);
            ret = (i >= 0) && (i < this->m_length);

            if(ret)
            {
                e = list_entry(position(i)->next, Node, head)->value;
            }

            return ret;

        }

        virtual T get(int i)const
        {
            T ret;

            if(get(i,ret))
            {
                return ret;
            }
            else
            {
                THROW_EXCEPTION(IndexOutOfBoundsException,"Invalid parameter i to get ...");
            }

            return ret;
        }

        int  find(const T& e )const//返回的是查找到的结点的位置
        {
            int ret = -1;
            int i = 0;
            list_head* slider = NULL;

            list_for_each(slider, &m_header)
            {
               if( list_entry(slider,Node,head)->value == e )
               {
                   ret = i;
                   break;
               }

               i++;
            }
            return ret;
        }

        int length() const
        {
            return this->m_length;
        }

        void clear()
        {
            while(length() > 0)
            {
                remove(0);
            }
        }

        bool move(int i, int step = 1)
        {
            bool ret = (i >= 0) && (i < this->m_length) && (step > 0);

            if(ret)
            {
                m_current = position(i)->next;
                this->m_step = step;//保存游标每次移动的数目
            }

            return ret;
        }

        bool end()
        {
            return (this->m_current == NULL) && (this->m_length == 0);
        }

        T current()
        {
            if(!end())
            {
                return list_entry(this->m_current, Node, head)->value;
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException,"No value at current position...");
            }
        }

        bool next()
        {
            int i = 0;
            while((i < this->m_step) && (!end()))
            {
                if(m_current != &m_header)
                {
                    m_current = m_current->next;
                    i++;
                }
                else
                {
                    m_current = m_current->next;
                }

            }

            if(m_current == &m_header)
            {
                m_current = m_current->next;
            }

            return (i == this->m_step);
        }

        bool pre()
        {
            int i = 0;
            while((i < this->m_step) && (!end()))
            {
                if(m_current != &m_header)
                {
                    m_current = m_current->prev;
                    i++;
                }
                else
                {
                    m_current = m_current->prev;
                }

            }

            if(m_current == &m_header)
            {
                m_current = m_current->prev;
            }

            return (i == this->m_step);
        }

        ~DualCircleList()
        {
            clear();
        }



    };

}


#endif // DUALCIRCLELIST_H
