#include <cstring>
#include <cstdlib>
#include "QSString.h"
#include "Exception.h"

namespace QSLib
{


int* String::make_pmt(const char* p)
{
     int len = strlen(p);//决定部分匹配表的长度
     int* ret = reinterpret_cast<int*>(malloc(sizeof(int) * len));//指向部分匹配表
     if(ret != NULL)
     {
         int ll = 0;

         ret[0] = 0;//长度为1的字符串的ll值为0

         for(int i = 1; i < len; i++ )//从长度为2的字符串开始计算，知道长度为len
         {
             while((ll > 0) && (p[ll] != p[i]))//挑选出一个可用的部分匹配值，根据部分匹配表的顺序位置挑选，直到部分匹配值为0，此时直接匹配首尾即可
             {
                 ll = ret[ll - 1];//找重叠部分的ll值，此时的ll值也就相当于重叠部分的长度，减一是为了匹配部分匹配表的下标
             }
             if(p[ll] == p[i])//新加进来的字符不能作为种子，找到种子并扩展一位后直接比对扩展的字符
             {
                 ll++;
             }
             ret[i] = ll;//在部分匹配表中存储部分匹配值
         }

     }


     return ret;

}

int String::kmp(const char* s, const char* p)
{
    int ret = -1;

    int sl = strlen(s);
    int pl = strlen(p);
    int* pmt = make_pmt(p);

    if((pmt != NULL) && (pl > 0) && (pl <= sl))
    {
        for(int i = 0, j = 0; i < sl; i++)
        {
            while((p[j] != s[i]) && (j > 0))
            {
                j = pmt[j - 1];//j的新值为ll值。
            }

            if(p[j] == s[i] )
            {
                j++;
            }
            if(j == pl)//匹配完成
            {
                ret = i + 1 - pl;//通过差量来确定起始位置
                break;
            }
        }
        free(pmt);
    }

    return ret;
}


//总初始化函数
void String::init(const char* s)
{
    m_str = strdup(s);//在堆中将s复制一份
    if( m_str )
    {
        m_length = strlen((const char*)m_str);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memeory to create String object...");
    }
}


//构造函数
String::String()
{
    init("");

}
String::String(char c)
{
    char s[] = {c, '\0'};
    init(s);
}
String::String(const char* s)
{
    init( s ? s : "");//防止被空指针初始化
}
String::String(const String& s)
{
    init(s.m_str);
}


//获取字符串长度
int String::length() const
{
    return m_length;
}

//将String对象转换成char*类型
const char* String::Str()const
{
    return this->m_str;
}


// []操作符重载
char& String::operator [](int i)//为什么是引用，是引用可以出现在赋值符号左边
{
    if( (i >= 0) && (i < m_length) )
    {
        return m_str[i];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException,"para i Index Out to use []...");

    }
}
char String::operator [](int i)const
{
    return (const_cast<String&>(*this)[i]);
}


//查找子串位置
int String::IndexOf(const String& s)const
{
    return kmp(m_str,s.m_str);
}
int String::IndexOf(const char* s)const
{
    return kmp(m_str,s ? s : "");//NULL指针处理方式
}


//删除从index开始的长为len的字符串
String& String::remove(int index, int len)
{
    if((index >= 0) && (index < m_length) && (len >= 0) && (len <= m_length))
    {
        int n = index;
        int m = index + len;

        //方法1
        strncpy(m_str+n, m_str+m, m_length - len - index);
        m_str[m_length - len] = '\0';
        m_length = m_length - len;

        //方法2
      /*  while((n < m) && (m < m_length))//m只是一个索引，最大值为m_length-1，不满足条件表示已经拷贝完成
        {
            m_str[n++] = m_str[m++];
        }
        m_str[n] = '\0';
        m_length = n;
      */
    }

    return *this;
}

String& String::remove(const String& s)
{
    return remove(IndexOf(s.m_str), s.length());
}
String& String::remove(const char* s)
{
    return remove(IndexOf(s),s ? strlen(s) : 0);
}


//子串替换
String& String::replace(const char* t, const char* s)
{
    int index = IndexOf(t ? t : "");
    if(index >= 0)
    {
        remove(t);
        insert(index, s ? s : "" );
    }
    return *this;
}

String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s ? s : "");
}

String& String::replace(const char* t, const String& s)
{
    return replace(t ? t : "", s.m_str);
}
String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}

String String::sub(int i, int len)const
{
    String ret;
    if( (i >= 0) && (i < m_length))
    {
        if(len < 0) len = 0;
        if(len + i > m_length) len = m_length - i;

        char* str = reinterpret_cast<char*>(malloc(sizeof(char) * (len + 1)));//多一个结束符空间

        strncpy(str, m_str + i, len);
        str[len] = '\0';
        ret = str;
    }
    else
    {
        THROW_EXCEPTION(InvalidOperationException, "parameter i is invaild");
    }
    return ret;

}

//功能函数实现

bool String::equal(const char* l, const char* r, int len)const
{
    bool ret = true;

    for(int i = 0; i < len && ret; i++)//只要检测到一个不相等就直接退出循环
    {
        ret = ret && (l[i] == r[i]);
    }
    return ret;
}
bool String::StartWith(const char* s)const//不需要改变成员变量
{
    bool ret = (s != NULL);
    if( ret )
    {
        int len = strlen(s);
        ret = (len <= m_length) && (equal(m_str, s, len));
    }

    return ret;
}
bool String::StartWith(const String& s) const
{
    return (StartWith(s.m_str));
}


bool String::EndOf(const char* s)const
{
    bool ret = (s != NULL);
    if( ret )
    {
        int len = strlen(s);
        char* start = m_str + (m_length - len);//定位到最后len长度的位置
        ret = (len <= m_length) && (equal(start, s, len));
    }

    return ret;
}
bool String::EndOf(const String& s)const
{
    return (EndOf(s.m_str));
}

//插入函数
String& String::insert(int i, const char* s)//链式操作，操作完成后返回的是字符串自己
{
    if( (i >= 0) && (i <= m_length))
    {
        if( (s != NULL) && (s[0] != '\0') )
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len + 1));
            if(str)
            {
                strncpy(str,m_str,i);
                strncpy(str + i, s, len );
                strncpy(str + i + len, m_str+i, m_length - i);

                str[m_length + len] = '\0';

                free(m_str);
                m_str = str;
                m_length = m_length + len;
            }
            else
            {
                 THROW_EXCEPTION(NoEnoughMemoryException,"No memory to insert ...");
            }
        }

    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException,"para i Index Out ...");
    }

    return *this;
}
String& String::insert(int i, const String& s)
{
    return insert(i,s.m_str);
}

//删除首尾空白字符
String& String::trim()
{
    int begin = 0;
    int end = m_length - 1;

    while(m_str[begin] == ' ') begin++;
    while(m_str[end]   == ' ') end--;

    if(begin == 0)
    {
        m_str[end + 1] = '\0';
        m_length = end + 1;
    }
    else
    {

        for(int i = 0, j = begin; j <= end; i++, j++ )
        {
            m_str[i] = m_str[j];
        }

        m_str[end - begin + 1] = '\0';
        m_length = end - begin + 1;
    }

    return *this;
}


// ==比较操作符重载
bool String::operator == (const String& s)const
{
    return strcmp(this->m_str,s.m_str) == 0;
}
bool String::operator == (const char* s)const
{
    return strcmp(this->m_str,s ? s : "") == 0;
}

// != 比较操作符重载
bool String::operator != (const String& s)const
{
    return !(this->m_str == s.m_str);
}
bool String::operator != (const char* s)const
{
    return !(this->m_str == s ? s : "");
}

// > 比较操作符重载
bool String::operator > (const String& s)const
{
    return strcmp(this->m_str,s.m_str) > 0;
}
bool String::operator > (const char* s)const
{
    return strcmp(this->m_str,s ? s : "") > 0;
}

// < 比较操作符重载
bool String::operator < (const String& s)const
{
    return strcmp(this->m_str,s.m_str) < 0;
}
bool String::operator < (const char* s)const
{
    return strcmp(this->m_str,s ? s : "") < 0;
}

// >= 比较操作符重载
bool String::operator >= (const String& s)const
{
    return strcmp(this->m_str,s.m_str) >= 0;
}
bool String::operator >= (const char* s)const
{
    return strcmp(this->m_str,s ? s : "") >= 0;
}

// <= 比较操作符重载
bool String::operator <= (const String& s)const
{
    return strcmp(this->m_str,s.m_str) <= 0;
}
bool String::operator <= (const char* s)const
{
    return strcmp(this->m_str,s ? s : "") <= 0;
}


// + 运算操作符重载
String String::operator + (const char* s)const//const函数不能直接改变成员变量的值但是可以使用另一个对象来改变对象的成员变量值
{
    String ret;

    int len = m_length + strlen( s ? s : "");
    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if(str)
    {
        strcpy(str,m_str);
        strcat(str,s ? s : "");

        free(m_str);

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memeory to mlalloc...");
    }


    return ret;
}
String String::operator + (const String& s)const
{
    return (*this + s.m_str);
}


// += 运算操作符重载
String& String::operator += (const String& s)
{
    return (*this = *this + s);
}
String& String::operator += (const  char* s)
{
    return (*this = *this + s);
}

//-运算符重载,要求是不能改变原字符串，并且需要返回一个新的字符串
String String::operator - (const String& s)const
{
    return String(*this).remove(s);//直接调用构造函数产生一个和this->m_str相等的临时对象，再调用remove函数
}
String String::operator - (const char* s)const
{
    return String(*this).remove(s);
}

//-=运算符重载,原字符串会被改变
String& String::operator -= (const String& s)//使用引用的原因是可以出现在赋值符的左边
{
    return remove(s);
}
String& String::operator -= (const  char* s)
{
    return remove(s);
}


// = 赋值运算符重载
String& String::operator = (const String& s)
{
    return (*this = s.m_str);
}
String& String::operator = (const  char* s)
{
    if(m_str != s)
    {
        char* str = strdup(s ? s : "");
        if(str)
        {
            free(m_str);
            m_str = str;
            m_length = strlen(m_str);

        }
        else
        {
           THROW_EXCEPTION(NoEnoughMemoryException, "No memeory to get in heap...");
        }
    }
    return *this;
}
String& String::operator = (const char c)
{
    char str[] = {c, '\0'};

    return (*this = str);
}


//析构函数
String::~String()
{
    free(m_str);
}


}
