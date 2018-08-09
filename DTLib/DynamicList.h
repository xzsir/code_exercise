#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H
#include "SeqList.h"


namespace QSLib
{
    template <typename T>
    class DynamicList : public SeqList<T>
    {
    protected:
        int m_capacity;//顺序存储的空间大小
    public:
        DynamicList(int capacity)//动态申请空间
        {
            this->m_array = new T[capacity];
            if(this->m_array != NULL)
            {
                this->m_length = 0;
                this->m_capacity = capacity;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException,"No memory to creat DynamicList object ...");
            }
        }


        int capacity() const
        {
            return m_capacity;
        }


        //重新设置顺序存储空间的大小，保证异常安全（异常发生后数据不会丢失）
        void resize(int capacity)
        {
            if(capacity != m_capacity )
            {
                T* array = new T[capacity]; //不直接操作this->m_array的目的就是保证原先的数据不会丢失

                if(array != NULL)
                {
                    int length = (this->m_length < capacity ? this->m_length : capacity);//长度为少的那个，不会出现越界的情况

                    for(int i = 0; i < length; i++)
                    {
                        //array多余的空间先空着等待数据插入
                        array[i] = this->m_array[i];//可能发生异常：由于T是泛指类型，当T是类类型时可能赋值操作符会被重载，而重载函数中含有出错代码的话就会发生异常
                    }

                    T* temp = this->m_array;//使用temp的原因：如果直接在此处使用delete[]this->m_array可能（T为类类型 ）触发析构函数调用而抛出一些异常，程序也会从当前语句返回，那后面的语句就不会执行了
                    //如何会抛出异常？
                    this->m_array = array;
                    this->m_length = length;
                    this->m_capacity = capacity;

                    delete []temp;
                }
                else
                {
                     THROW_EXCEPTION(NoEnoughMemoryException,"No memory to resize DynamicList object ...");
                }


            }
        }


        ~DynamicList()//归还空间
        {
            delete [] this->m_array;
        }
    };
}


#endif // DYNAMICLIST_H
