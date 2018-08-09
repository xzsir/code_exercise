#ifndef DUALSTATICLINKLIST_H
#define DUALSTATICLINKLIST_H


#include "DualLinkList.h"
namespace QSLib
{
    template <typename T, int N>
    class DualStaticLinkList : public DualLinkList<T>
    {
    protected:
        typedef typename DualLinkList<T>::Node Node;//要想使用LinkList的Node需要指明作用域，
                                                //但是这又导致编译器不知道它代表类型还是静态成员变量，所以需要加上typename

        struct SNode : public Node
        {
            void* operator new (unsigned int size, void* loc)
            {
                (void)size;//size的处理方式，不然编译器会提示未使用
                return loc;
            }
        };
        unsigned char m_space[sizeof(SNode) * N];
        int m_used[N];

         virtual Node* create()
        {
            SNode* ret = NULL;

            for(int i = 0; i < N; i++ )
            {
                if(!m_used[i])
                {
                    ret = reinterpret_cast<SNode*>(m_space) + i;
                    ret = new(ret)SNode();//在ret处调用构造函数，如果为非类类型则只返回空间ret
                    m_used[i] = 1;
                    break;
                }
            }

            return ret;
        }
        virtual void destroy(Node* pn)
        {
            SNode* space = reinterpret_cast<SNode*>(m_space);
            SNode* psn = dynamic_cast<SNode*>(pn);
            for(int i = 0; i < N; i++)
            {
                if( psn == (space + i) )
                {
                    m_used[i] = 0;
                    psn->~SNode();
                    break;
                }
            }
        }

    public:
        DualStaticLinkList()
        {
            for(int i = 0; i < N; i++)
            {
                m_used[i] = 0;
            }
        }

        int capacity()
        {
            return N;
        }

        ~DualStaticLinkList()
        {
            this->clear();//构造函数和析构函数中不能发生多态，只能调用当前类中的虚函数实现版本，所以如果此处不添加析构函数的话就会调用到父类的destroy函数
        }
    };
}

#endif // DUALSTATICLINKLIST_H
