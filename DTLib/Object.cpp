#include <iostream>
#include "object.h"
#include <cstdlib>
/*现代的C++编译器new失败会抛出异常，在不同的编译器可能表现得也不同，由于我们想在大部分编译器下获取同样的行为所以我们重载new和delete*/

using namespace std;
namespace QSLib
{
    void* Object::operator new(unsigned int size) throw()//表示不抛出任何异常
    {
        //cout << "Object::operator new(unsigned int size) :" << size << endl;
        return malloc(size);
    }
    void  Object::operator delete(void* p) throw()
    {
        //cout << "Object::operator delete(void* p)" << p << endl;
        free(p);
    }
    void* Object::operator new[](unsigned int size) throw()
    {
        return malloc(size);
    }
    void  Object::operator delete[](void* p) throw()
    {
        free(p);
    }

    bool Object::operator ==(const Object& obj)
    {
        return (this == &obj);
    }
    bool Object::operator !=(const Object& obj)
    {
        return (this != &obj);
    }

    Object::~Object()
    {


    }
}
