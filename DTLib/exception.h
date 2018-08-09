#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Object.h"

namespace QSLib
{

#define THROW_EXCEPTION(e,m) (throw e(m, __FILE__, __LINE__))
class Exception : public Object
{
protected:
    char* m_message;//说明当前异常的详细信息
    char* m_location;//说明发生异常的地点

    void init(const char* message,const char* file,int line);

public:
    Exception(const char* message);
    Exception(const char* file, int line);
    Exception(const char* message,const char* file,int line);

    Exception(const Exception& e);
    Exception& operator= (const Exception& e);

    virtual const char* message() const;
    virtual const char* location() const;

    virtual ~Exception() = 0;//C++中规定但凡手动提供了析构函数就一定要实现，因为析构一个子类对象时一定会析构父类，不关心此处的纯虚函数在子类中实现
};

class ArithmeticException : public Exception
{
public:
    ArithmeticException() : Exception(0){}
    ArithmeticException(const char* message) : Exception(message){}
    ArithmeticException(const char* file,int line) : Exception(file,line){}
    ArithmeticException(const char* message,const char* file,int line) : Exception(message,file,line){}

    ArithmeticException(const ArithmeticException& e) : Exception(e){}

    ArithmeticException& operator= (const ArithmeticException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NullPointerException : public Exception
{
public:
    NullPointerException() : Exception(0){}
    NullPointerException(const char* message) : Exception(message){}
    NullPointerException(const char* file,int line) : Exception(file,line){}
    NullPointerException(const char* message,const char* file,int line) : Exception(message,file,line){}

    NullPointerException(const NullPointerException& e) : Exception(e){}

    NullPointerException& operator= (const NullPointerException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException() : Exception(0){}
    IndexOutOfBoundsException(const char* message) : Exception(message){}
    IndexOutOfBoundsException(const char* file,int line) : Exception(file,line){}
    IndexOutOfBoundsException(const char* message,const char* file,int line) : Exception(message,file,line){}

    IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e){}

    IndexOutOfBoundsException& operator= (const IndexOutOfBoundsException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

class NoEnoughMemoryException : public Exception
{
public:
    NoEnoughMemoryException() : Exception(0){}
    NoEnoughMemoryException(const char* message) : Exception(message){}
    NoEnoughMemoryException(const char* file,int line) : Exception(file,line){}
    NoEnoughMemoryException(const char* message,const char* file,int line) : Exception(message,file,line){}

    NoEnoughMemoryException(const NoEnoughMemoryException& e) : Exception(e){}

    NoEnoughMemoryException& operator= (const NoEnoughMemoryException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};


class InvalidParameterException : public Exception
{
public:
    InvalidParameterException() : Exception(0){}
    InvalidParameterException(const char* message) : Exception(message){}
    InvalidParameterException(const char* file,int line) : Exception(file,line){}
    InvalidParameterException(const char* message,const char* file,int line) : Exception(message,file,line){}

    InvalidParameterException(const InvalidParameterException& e) : Exception(e){}

    InvalidParameterException& operator= (const InvalidParameterException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};

//在不正常状态的时候调用函数抛出异常
class InvalidOperationException : public Exception
{
public:
    InvalidOperationException() : Exception(0){}
    InvalidOperationException(const char* message) : Exception(message){}
    InvalidOperationException(const char* file,int line) : Exception(file,line){}
    InvalidOperationException(const char* message,const char* file,int line) : Exception(message,file,line){}

    InvalidOperationException(const InvalidOperationException& e) : Exception(e){}

    InvalidOperationException& operator= (const InvalidOperationException& e)
    {
        Exception::operator=(e);

        return *this;
    }
};


}


#endif // EXCEPTION_H
