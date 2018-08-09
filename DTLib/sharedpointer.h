#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H
#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"

namespace QSLib {

    template <typename T>
    class SharedPointer : public Pointer<T>
    {
      protected:
        int* m_ref;

        void assign(const SharedPointer<T>& obj)
        {
            this->m_ref = obj.m_ref;
            this->m_pointer = obj.m_pointer;
            if(this->m_ref)
            {
               ( *this->m_ref)++;
            }
        }

    public:
          SharedPointer(T* p = NULL) : m_ref(NULL)
           {
               if(p)
               {
                   this->m_ref = static_cast<int*>(malloc(sizeof(int)));
                   if(this->m_ref)
                   {
                       (*this->m_ref) = 1;//申请空间成功后表示已经有指针指向它，计数变量加1。
                       this->m_pointer = p;
                   }
                   else//抛出异常
                   {
                       THROW_EXCEPTION(NoEnoughMemoryException,"No enough memory to create...");
                   }

               }
           }

           SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)//拷贝构造，拷贝成功后计数变量加1
           {
               this->m_ref = obj.m_ref;
               this->m_pointer = obj.m_pointer;
               if(this->m_ref)
                   (*this->m_ref)++;
           }

           SharedPointer<T>& operator= (const SharedPointer<T>& obj)//赋值操作符重载，赋值成功后计数变量加1
           {
               if(this != &obj)
               {
                   clear();//清理当前指向
                   this->m_pointer = obj.m_pointer;//重新赋值
                   this->m_ref = obj.m_ref;

                   (*this->m_ref)++;
               }
               return *this;
           }
           void clear()//清除当前对象指向的空间，当计数变量为0时释放堆空间和计数变量占用的空间
           {
               T* toDel = this->m_pointer;
               int* ref = this->m_ref;//

               this->m_pointer = NULL;
               this->m_ref = NULL;//两步操作执行后计数变量数值不变。
               if(ref)//清理指向堆空间的指针
               {
                  ( *ref)--;

                   if((*ref) == 0)
                   {
                       free(ref);
                       delete(toDel);

                   }
               }
           }
           ~SharedPointer()
           {
               std::cout << "SharedPointer clear start" << std::endl;
               clear();
               std::cout << "SharedPointer clear end" << std::endl;

           }

       };
       //比较操作符重载，作为全局重载函数
       template <typename T>
       bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
       {
           return (l.get() == r.get());
       }

       template <typename T>
       bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)//const对象只能调用const成员函数
       {
           return !(l == r);
       }

}


#endif // SHAREDPOINTER_H
