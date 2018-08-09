#ifndef STACK_H
#define STACK_H
#include "Object.h"


namespace QSLib {
template <typename T>
class Stack : public Object
{
public:
    virtual void push(const T& obj) = 0;
    virtual void pop() = 0;
    virtual int size()const = 0;//const对象调用
    virtual T top()const = 0;//const 对象调用
    virtual void clear() = 0;

};

}



#endif // STACK_H
