#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Object.h"

#include "Pointer.h"

/*智能指针要求：指针生命周期结束时主动释放堆空间，一片堆空间只能由一个指针标识，杜绝指针运算和指针比较，只能指向堆空间的单个对象或变量，不能指向数组或局部变量*/

namespace QSLib
{


//改良后的智能指针
 template <typename T>
 class SmartPointer : public Pointer<T>
 {

 public:
     SmartPointer(T* p = NULL) : Pointer<T>(p)
     {

     }

     SmartPointer(const SmartPointer<T>& obj)
     {
         this->m_pointer = obj.m_pointer;
         const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;//曾经出过错，将(obj).m_pointer写成(obj.m_pointer)
     }

     SmartPointer<T>& operator =(const SmartPointer<T>& obj)
     {
         if(this != &obj)
         {
             T* p = this->m_pointer;//使用this指针限定作用域
             this->m_pointer = obj.m_pointer;
             const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;

             delete p;
         }
         return *this;//返回的意义在于可以进行连续赋值
     }

     ~SmartPointer()
     {
         delete this->m_pointer;
     }

 };
/***************************第一版智能指针***********************************************/
/* template <typename T>
 class SmartPointer : public Object
 {
 protected:
     T* m_pointer;

 public:
     SmartPointer(T* p = NULL)
     {
        m_pointer = p;
     }
     SmartPointer(const SmartPointer<T>& obj)
     {
         m_pointer = obj.m_pointer;
         const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;//曾经出过错，将(obj).m_pointer写成(obj.m_pointer)
     }

     SmartPointer<T>& operator =(const SmartPointer<T>& obj)
     {
         if(this != &obj)
         {
             delete m_pointer;
             m_pointer = obj.m_pointer;
             const_cast<SmartPointer<T>&>(obj).m_pointer = NULL;
         }
         return *this;//返回的意义在于可以进行连续赋值
     }
     T* operator -> ()
     {
           return m_pointer;
     }

     T& operator* ()
     {
        return *m_pointer;
     }
     bool isNull()
     {
         return (m_pointer == NULL);
     }
     T* get()
     {
           return m_pointer;
     }
     ~SmartPointer()
     {
         delete m_pointer;
     }

 };
 */
 /****************************************end*********************************************************************/



}

#endif // SMARTPOINTER_H
