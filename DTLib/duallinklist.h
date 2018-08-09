#ifndef DUALLINKLIST_H
#define DUALLINKLIST_H
#include "List.h"
#include "Exception.h"

namespace QSLib
{
    template <typename T>
    class DualLinkList : public List<T>
    {
    protected:
        struct Node : public Object
        {
            T value;
            Node* next;
            Node* pre;

        };
        //mutable Node m_header;//分析下面的匿名结构的作用：本质上就是防止调用创建T value对象时调用构造函数，下面的匿名结构在内存布局上和Node m_header布局相同
       mutable struct : public Object//如果未继承自Object可能导致内存布局和Node m_header内存布局不同。
        {

            char reserved[sizeof(T)];
            Node* next;
            Node* pre;

        }m_header;
        int m_length;

        int m_step;//保存游标移动的次数
        Node* m_current;//游标

        Node* position(int i)const//用于定位 ，优化insert、remove、get、set函数用，但是本文件未优化，便于复习使用，这里只是说明可以优化
        {
            Node* current = reinterpret_cast<Node*>(&m_header);

            for(int p = 0; p < i; p++)
            {
                current = current->next;
            }
            return current;
        }

        virtual Node* create()
        {
            return new Node();
        }

        //使用智能指针后不需要destroy
        virtual void destroy(Node* pn)
        {

            delete pn;
        }


    public:
        DualLinkList()
        {
            m_header.next = NULL;
            m_header.pre = NULL;
            m_length = 0;

            m_step = 1;
            m_current = NULL;
        }


        bool insert(int i,const T& e)
        {
            bool ret = (i >= 0) && (i <= m_length);

            if(ret)
            {
                Node* node = create();

                if(node != NULL)
                {

                    Node* current = reinterpret_cast<Node*>(&m_header);


                    //定位到要插入的位置
                    for(int p = 0; p < i; p++)
                    {
                        current = current->next;
                    }

                    Node* next = current->next;
                    node->value = e;

                    //连接next域

                    //第一二步
                    node->next = next;
                    current->next = node;

                    //连接pre域

                    //第三四步
                    if(current != reinterpret_cast<Node*>(&m_header))
                    {
                        node->pre = current;
                    }
                    else
                    {
                        node->pre = NULL;
                    }
                    if(next != NULL)
                    {
                        next->pre = node;
                    }

                    m_length++;

                }
                else
                {
                    THROW_EXCEPTION(NoEnoughMemoryException,"No memory to new ");
                }
            }

            return ret;
        }

        bool insert(const T& e)
        {
            return insert(m_length,e);
        }

        bool remove(int i)
        {
            bool ret = (i >= 0) && (i < m_length);

            if(ret)
            {
                Node* current = reinterpret_cast<Node*>(&m_header);//出过错

                for(int p = 0; p < i; p++)
                {
                    current = current->next;
                }

                Node* toDel = current->next;
                Node* next = toDel->next;

                if(m_current == toDel)//作用：在遍历中执行remove操作时删除结点后会导致m_current指向不变，从而使m_current->value为随机值，所以当需要删除将m_current指向下一个结点
                {
                    m_current = next;
                }

                //第一步
                current->next = next;
                //第二步
                if(next != NULL)
                {
                    next->pre = toDel->next;//出过BUG，写法：next->pre = current;
                }


                m_length--;//保证异常安全，因为当销毁数据时抛出异常（结点是类类型，并且在析构函数中抛出异常）先长度减一再销毁结点

                destroy( toDel );


            }
            return ret;
        }
        bool set(int i,const T& e)
        {
            bool ret = (i >= 0) && (i < m_length);

            if(ret)
            {
                Node* current = reinterpret_cast<Node*>(&m_header);
                for(int p = 0; p < i; p++)
                {
                    current = current->next;
                }
                current->next->value = e;
            }

            return ret;
        }
        bool get(int i,T& e )const
        {
            bool ret = (i >= 0) && (i < m_length);

            if(ret)
            {
                Node* current = reinterpret_cast<Node*>(&m_header);

                for(int p = 0; p < i; p++)
                {
                    current = current->next;
                }
                e = current->next->value;
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
            Node* node  = m_header.next;

            while(node)
            {
                if((node->value) == e)
                {
                    ret = i;
                    break;
                }
                else
                {
                    node = node->next;
                    i++;
                }
            }
            return ret;
        }

        int length()const
        {
            return m_length;
        }


        void clear()
        {
            while(m_length > 0)
            {
                remove(0);
            }

        }
/**************************************单链表编译函数组*********************************************************/

        /*为什么需要实现单链表遍历，为什么不直接通过for循环直接get数据，因为这样效率低，时间复杂度有O(n^2)
           我们希望能够线性获取数据元素，实现了四个函数：
            move：将游标定位到i，设置每次移动step个位置。
            end:  判断游标是否指向NULL
            current：返回游标指向的value
            next：移动游标，移动m_step个位置*/
        virtual bool move(int i, int step = 1)
        {
            bool ret = (i >= 0) && (i < m_length) && (step > 0);

            if(ret)
            {
                m_current = position(i)->next;
                m_step = step;//保存游标每次移动的数目
            }


            return ret;
        }

        virtual bool end()
        {
            return (m_current == NULL);
        }

       virtual  T current()
        {
            if(!end())
            {
                return m_current->value;
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException,"No value at current position...");
            }
        }

        virtual bool next()
        {
            int i = 0;
            while((i < m_step) && (!end()))
            {
                m_current = m_current->next;
                i++;
            }

            return (i == m_step);
        }

        virtual bool pre()
        {
            int i = 0;
            while((i < m_step) && (!end()))
            {
                m_current = m_current->pre;
                i++;
            }

            return (i == m_step);
        }

/*******************************************end***************************************************************/

        ~DualLinkList()
        {
            //std::cout << "~LinkList1" << std::endl;
            this->clear();//析构函数中不能实现多态，只能调用当前类里实现的虚函数版本
            //std::cout << "~LinkList2" << std::endl;
        }
    };

}

#endif // DUALLINKLIST_H
