#ifndef QSSTRING_H
#define QSSTRING_H
#include "Object.h"

namespace QSLib
{
class String : public Object
{
protected:
    char* m_str;
    int m_length;
    void init(const char* s);
    bool equal(const char* l, const char* r, int len)const;
    static int* make_pmt(const char* p);
    static int kmp(const char* s, const char* p);//能够直接通过类名调用，好处是可以不依赖String类对象也可以进行子串查找




public:
    String();
    String(char c);
    String(const char* s);
    String(const String& s);

    int length() const;
    const char* Str() const;

    //子串查找
    int IndexOf(const String& s)const;
    int IndexOf(const char* s)const;


    //子串删除
    String& remove(int index, int len);
    String& remove(const String& s);
    String& remove(const char* s);

    //子串替换
    String& replace(const char* t, const char* s);
    String& replace(const String& t, const char* s);
    String& replace(const char* t, const String& s);
    String& replace(const String& t, const String& s);

    //子串创建
    String sub(int i, int len)const;


    bool StartWith(const char* s)const;//不需要改变成员变量
    bool StartWith(const String& s) const;
    bool EndOf(const char* s)const;
    bool EndOf(const String& s)const;

    //插入函数
    String& insert(int i, const char* s);//链式操作，操作完成后返回的是字符串自己 `
    String& insert(int i, const String& s);

    //删除字符串前后的空白
    String& trim();

    char& operator [](int i);//返回值为引用表示可以作为左值，给非const对象使用
    char operator [](int i)const;
    bool operator == (const String& s)const;
    bool operator == (const char* s)const;
    bool operator != (const String& s)const;
    bool operator != (const char* s)const;
    bool operator > (const String& s)const;
    bool operator > (const char* s)const;
    bool operator < (const String& s)const;
    bool operator < (const char* s)const;
    bool operator >= (const String& s)const;
    bool operator >= (const char* s)const;
    bool operator <= (const String& s)const;
    bool operator <= (const char* s)const;


    String operator + (const String& s)const;
    String operator + (const char* s)const;
    String& operator += (const String& s);//使用引用的原因是可以出现在赋值符的左边
    String& operator += (const  char* s);

    String operator - (const String& s)const;
    String operator - (const char* s)const;
    String& operator -= (const String& s);//使用引用的原因是可以出现在赋值符的左边
    String& operator -= (const  char* s);

    String& operator = (const String& s);
    String& operator = (const  char* s);
    String& operator = (const char c);


    ~String();



};

}


#endif // QSSTRING_H
