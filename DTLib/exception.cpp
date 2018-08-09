#include "Exception.h"
#include <cstring>
#include <cstdlib>

using namespace std;

namespace QSLib
{
    void Exception::init(const char* message,const char* file,int line)
    {
        m_message = (message ? strdup(message) : NULL);//直接赋值不安全，无法确定message存储在堆、栈、还是全局数据区，无法确定message指向的外部字符串的生命周期，使用strdup将字符串拷贝一份放在了堆空间
        //判断message是否为空的原因：在标准库中对strdup函数的实现并没有判断函数参数是否为空，直接默认为不为空。这样才足够安全
        if(file != NULL)
        {
            char sl[16] = {0};

            itoa(line,sl,10);

            m_location = static_cast<char*>(malloc(strlen(file) + strlen(sl) + 2));

            if(m_location != NULL)//这里最好的处理方法就是不要else分支，因为下面已经作了申请失败的处理
            {
                m_location = strcpy(m_location,file);
                m_location = strcat(m_location,":");
                m_location = strcat(m_location,sl);
            }

        }
        else
        {
            m_location = NULL;
        }

    }

    /******构造函数*********/
    Exception :: Exception(const char* message)
    {
        init(message,NULL,0);
    }
    Exception :: Exception(const char* file, int line)
    {
        init(NULL,file,line);
    }
    Exception :: Exception(const char* message,const char* file,int line)
    {
        init(message,file,line);
    }

    Exception :: Exception(const Exception& e)
    {
        m_message  = strdup(e.m_message);//都指向堆空间的一段独立的内存，实现深拷贝
        m_location = strdup(e.m_location);
    }

    /**赋值操作符重载*/
    Exception& Exception :: operator= (const Exception& e)
    {
        if(this != &e)//判断自赋值
        {
            free(m_message);
            free(m_location);//根据赋值的原义，赋值操作后原先的空间就不再使用

            m_message  = strdup(e.m_message);//都指向堆空间的一段独立的内存，实现深拷贝
            m_location = strdup(e.m_location);
        }

        return *this;
    }

    const char* Exception :: message() const
    {
        return m_message;
    }
    const char* Exception :: location() const
    {
        return m_location;
    }

    Exception :: ~Exception()
    {
        free(m_message);
        free(m_location);
    }


}
