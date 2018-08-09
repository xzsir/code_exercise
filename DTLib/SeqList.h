#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "Exception.h"

namespace QSLib
{
    template <typename T>
    class SeqList : public List<T>//类模板在外部使用都要带上类型
    {
    protected:
        T* m_array;//顺序存储空间
        int m_length;//当前表长

    public:

        bool insert(int i,const T& e)
        {
            bool ret = false;

            ret = (i >= 0) && (i <= m_length);//为什么可以 = m_length？五个指头，可以有六个放笔的地方，插入元素也一样。
            ret = ret && (m_length + 1 <= capacity());

            if(ret)
            {

                for(int j = m_length - 1; j >= i; j--)
                {
                    m_array[j+1] = m_array[j];
                }
                m_array[i] = e;//出过错，错写成：m_array[j] = e;

                m_length++;
            }
            return ret;
        }
        bool insert(const T& e)//在尾部插入
        {
            return insert(m_length,e);
        }


        bool remove(int i)
        {
            bool ret = (i >= 0) && (i < m_length);

            if(ret)
            {
                for(int j = i; j < m_length - 1; j++)
                {
                    m_array[j] = m_array[j+1];
                }

                m_length--;
            }

            return ret;
        }

        bool set(int i, const T& e)
        {
            bool ret = (i >= 0) && (i < m_length);

            if(ret)
            {
                m_array[i] = e;
            }

            return ret;
        }
        bool get(int i, T& e) const  // 为什么使用引用将返回的值存到引用里？ 当i不合法时函数就返回一个false，可以判断程序是否正常 //为什么用const，防止被意外改变。
        {
            bool ret = (i >= 0) && (i <= m_length);

            if(ret)
            {
                e = m_array[i];
            }

            return ret;
        }

        int  find(const T& e )const //O(n)
        {
            int ret = -1;
            for(int i = 0; i < m_length; i++)
            {
                if(m_array[i] == e)
                {
                    ret = i;
                    break;
                }
            }

            return ret;
        }
        int length() const
        {
            return m_length;
        }
        void clear()
        {
            m_length = 0;
        }
        //顺序存储线性表的数组访问方式
        T& operator[](int i)
        {
            if((i >= 0) && (i < m_length))
            {
                return m_array[i];
            }
            else
            {
                THROW_EXCEPTION(IndexOutOfBoundsException,"parameter i is IndexOutOfBounds...");
            }
        }
        T  operator[](int i) const
        {
            return (const_cast<SeqList<T>&>(*this))[i];

        }
        //顺序存储空间的容量
        virtual int capacity() const = 0;//由于容量和类型都是在子类中实现，所以作为纯虚函数
    };
}


#endif // SEQLIST_H
