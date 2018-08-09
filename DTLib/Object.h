#ifndef OBJECT_H
#define OBJECT_H


/*Object类是DTLib中数据结构类的顶层父类
  用于同一类的动态内存申请的行为
  在堆中创建Object子类的对象，失败时返回NULL
  Object类为纯虚父类，所有子类都能进行动态类型识别，类型识别分为动态 和静态，静态是基本类型加上不含虚函数的类类型，动态是含有继承虚函数继承的类类型
   使用自定义类时一般来继承Object类避免一些编译错误*/
namespace QSLib
{

    class Object
    {
    public:
        void* operator new(unsigned int size) throw();
        void  operator delete(void* p) throw();
        void* operator new[](unsigned int size) throw();
        void  operator delete[](void* p) throw();
        bool operator ==(const Object& obj);
        bool operator !=(const Object& obj);
        virtual ~Object() = 0;
    };
}



#endif // OBJECT_H
