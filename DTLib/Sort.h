#ifndef SORT_H
#define SORT_H
#include "Object.h"

namespace QSLib
{
class Sort : public Object
{
private:
    Sort();//将构造函数私有化就是禁止创建对象
    Sort(const Sort& obj);
    Sort& operator =(const Sort& obj);
    template <typename T>
    static void Swap(T& a, T& b)
    {
        T c(a);//初始化
        a = b;
        b = c;
    }

public:

    //选择排序，可以同时支持从小到大或从大到小
    //默认从小到大
    template <typename T>
    static void Select(T arr[], int len, bool min2max = true)
    {
        for(int i = 0; i < len; i++)//交换第i个位置上的数据
        {
            int MinOrMax = i;
            for(int j = i+1; j < len; j++)//从第i+1个数据开始比较
            {
                if(min2max ? (arr[MinOrMax] > arr[j]) : (arr[MinOrMax] < arr[j]))//测试标识以哪个顺序排序
                {
                    MinOrMax = j;
                }

            }
            if(MinOrMax != i)
            {
                Swap(arr[i],arr[MinOrMax]);
            }
        }

    }


    //插入排序，可以同时支持从小到大或从大到小
    //默认从小到大
    template <typename T>
    static void Insert(T arr[], int len, bool min2max = true)
    {
        for(int i = 1; i < len; i++)//将第i个数据往有序序列中插入
        {
            T in = arr[i];//拿出待插入的元素
            int k = i;
            //将待插入数据同有序序列比较来找到合适位置插入
            //从后向前比较
            for(int j = i-1; (j >= 0) && (min2max ? (arr[j] > in) : (arr[j] < in)); j--)
            {
                arr[j+1] = arr[j];//后移数据
                k = j;
            }
            if(k != i)//位置未变则不进行复制操作
            {
                arr[k] = in;
            }

        }
    }


    //冒泡排序，可以同时支持从小到大或从大到小
    //默认从小到大
    template <typename T>
    static void Bubble(T arr[], int len, bool min2max = true)
    {
        bool exchange = true;
        for(int i = 0; (i < len) && exchange; i++)//进行冒泡的次数
        {
            exchange = false;
            for(int j = len - 1; j > i; j--)//j>i表示已经排好序的不再管
            {
                if( min2max ? (arr[j - 1] > arr[j]) : (arr[j - 1] < arr[j]) )
                {
                    Swap(arr[j - 1], arr[j]);
                    exchange = true;
                }
            }
        }
    }


    //希尔排序，可以同时支持从小到大或从大到小
    //默认从小到大

    //分组进行插入排序，最后进行一次总的插入排序
    //分组大小d从len开始减小，直到为1，每一个d值在当前序列分组中都会被分成d个序列（可以写出分组序列进行验证）
    //原理：1、先确定d的大小，再分别找出R[1]、R[1+d]、R[1+2d]...然后将这些序列进行插入排序
    //     2、d自减1，再分别找出R[2]、R[2+d]、R[2+2d]...然后将这些序列进行插入排序，起始从1+d开始划分，一直到d+d划分结束
    //     3、持续分组，直到d的值为1.
    //     4、最后在当d == 1时进行对基本有序序列的基础上进行一次插入排序
    template <typename T>
    static void Shell(T arr[], int len, bool min2max = true)//O(n^3/2)
    {
        int d = len;//当d为len时每个序列只有一个元素，不需要排序，直接跳过
        do//每一次的循环表示进行一次分组，然后插入排序
        {
            d = (d / 3) + 1;//数学公式，比单纯的d--效率高
            //为什么i是从d开始而不是从1开始？间隔为d所以从d开始，每次加d，这是对分组后的序列进行插入排序
            //在d>1之前先分组排序形成基本有序序列，最后在基本有序的基础上进行插入排序
            for(int i = d; i < len; i += d)
            {
                T in = arr[i];
                int k = i;
                //将待插入数据同有序序列比较来找到合适位置插入
                for(int j = i-d; (j >= 0) && (min2max ? (arr[j] > in) : (arr[j] < in)); j -= d)
                {
                    arr[j+d] = arr[j];//后移数据，移动间隔为d，因为当前进行插入排序的序列是R[i]、R[i+d]、R[i+2d]...
                    k = j;
                }
                if(k != i)//位置未变则不进行复制操作
                {
                    arr[k] = in;
                }

            }

        }while(d > 1);//不能以d > 0作为结束条件，当d==1时进行最后一次插入排序，但是根据d的数学公式来看d至少为1。
    }

    //希尔排序
    //以冒泡排序作为基础排序算法
    template <typename T>
    static void Shell_pro(T arr[], int len, bool min2max = true)//O(n^3/2)
    {
        int d = len;
        do
        {
            d = (d / 3) + 1;
            bool exchange = true;

            //冒泡排序
            for(int i = d-1; (i < len) && exchange; i+=d)
            {
                exchange = false;
                for(int j = len - d; j > i; j-=d)
                {
                    if( min2max ? (arr[j - d] > arr[j]) : (arr[j - d] < arr[j]) )
                    {
                        Swap(arr[j - d], arr[j]);
                        exchange = true;
                    }
                }
            }



        }while(d > 1);
    }

};

}


#endif // SORT_H
