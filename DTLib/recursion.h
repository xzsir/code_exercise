#ifndef RECURSION_H
#define RECURSION_H
#include <iostream>
#include <cstring>
#include "Exception.h"
#include "LinkList.h"

using namespace std;


namespace QSLib
{

struct Node1
{
    int value;
    Node1* next;
};

Node1* create_list(int v, int len)
{
    Node1* ret = NULL;
    Node1* slider = NULL;
    for(int i = 0; i < len; i++)
    {
        Node1* n = new Node1();

        n->value = v++;
        n->next = NULL;
        if(slider == NULL)
        {
            slider = n;
            ret = n;
        }
        else
        {
            slider->next = n;
            slider = n;
        }
    }

    return ret;
}


void destroy_list(Node1* list)
{
    while(list)
    {
        Node1* del = list;
        list = list->next;
        delete del;
    }
}

void print_list(Node1* list)
{
    while(list)
    {
        cout << list->value << "->";
        list = list->next;
    }

    cout << "NULL" << endl;
}

Node1* reserve(Node1* list)//无头结点的链表
{
    //先找出口
    if(list == NULL || list->next == NULL)
    {
        return list;
    }
    else
    {
        Node1* guard = list->next;
        Node1* ret = reserve(list->next);
        guard->next = list;
        list->next = NULL;
        return ret;
    }


}

//合并两个链表
Node1* merge(Node1* list1, Node1* list2)
{
    //第一步，找出口
    if(list1 == NULL)
    {
        return list2;
    }
    else if(list2 == NULL)
    {
        return list1;
    }
    else if(list1->value < list2->value)//都不为空，且list1的首元素值比较小3

    {
        Node1* _list1 = list1->next;
        list2 = merge(_list1, list2);

        list1->next = list2;

        return list1;
    }
    else
    {
        Node1* _list2 = list2->next;
        list1 = merge(_list2, list1);

        list2->next = list1;

        return list2;
    }

}

//思路：先将问题简化，打印从第二个开始的子表中偶数节点，最后打印第一个节点
//根据栈的规则，必定是第一个节点最后打印，根据这个特性就能实现逆序打印
void re_print(Node1* list)
{
    if(list != NULL)
    {
        re_print(list->next);

        if(list->value % 2 == 0)
        {
            cout << list->value << endl;
        }
    }
}

//汉诺塔
void HanoiTower(int num, char a, char b, char c)
{
    //第一步，找出口
    if(num == 1)
    {
        cout << a << "--->" << c << endl;
    }
    else
    {
        HanoiTower(num - 1, a, c, b);//将num - 1个盘子从a经过c移动到b
        HanoiTower(1, a, b, c);//将第num个盘子从a经过b移动到c
        HanoiTower(num - 1, b, a, c);//将num - 1个盘子从a经过c移动到b
    }
}


//全排列
void permutation(char* str, char* start)
{
    //第一步先找出口
    if(*str == '\0')
    {
        cout << start << endl;
    }
    else
    {
        int len = strlen(str);
        for(int i = 0; i < len; i++)
        {
            if( i == 0 || str[0] != str[i])
            {
                swap(str[0], str[i]);
                permutation(str + 1, start);
                swap(str[0], str[i]);

            }

        }
    }
}




//实现八皇后问题，使用类模板编程，能够实现N皇后问题
template<int SIZE>
class QueueQuestion: public Object
{
protected:
    enum{N = SIZE + 2};//定义数组范围
    struct pos : public Object//定义棋盘位置
    {
      pos(int px = 0, int py = 0): x(px), y(py){ }
      int x;
      int y;
    };


    int m_chessboard[N][N]; //创建N*N大小的棋盘
    pos m_dir[3];//定义三个方向坐标
    LinkList<pos> m_solution;//定义一个链表来保存解决方法，即可放皇后的位置坐标，pos类型数据节点
    int m_count;

    //初始化棋盘
    void chessinit()
    {
        m_count = 0;

        //标记边界
        for(int i = 0; i < N; i += (N-1))//表示第一列和最后一列
        {
            for(int j = 0; j < N; j++)
            {
                m_chessboard[i][j] = 2;//标记左右边界
                m_chessboard[j][i] = 2;//标记上下边界
            }
        }

        //标记中间位置
        for(int i = 1; i <= SIZE; i++)
        {
            for(int j = 1; j <= SIZE; j++)
            {
                m_chessboard[i][j] = 0;
            }
        }

        //初始化方向坐标，在皇后问题中只需要检测三个方向即可，左下角、正下方、右下角。其余方向不可能用得着，因为我们是按照顺序来进行放置皇后的。
        m_dir[0].x = -1;//左下角
        m_dir[0].y = -1;

        m_dir[1].x = 0;//正下方
        m_dir[1].y = -1;

        m_dir[2].x = 1;//右下角
        m_dir[2].y = -1;



    }
    //打印棋盘
    void chess_print()
    {
        //打印出可解决的方法
        for(m_solution.move(0); !m_solution.end(); m_solution.next())
        {
            cout << "(" << m_solution.current().x << "," << m_solution.current().y << ")";
        }

        cout << endl;

        //打印出棋盘
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                switch(m_chessboard[i][j])
                {
                    case 0: cout << " "; break;
                    case 1: cout << "#"; break;
                    case 2: cout << "*"; break;
                }
            }
            cout << endl;
        }
        cout << endl;


    }

    //返回真表示在该方向上没有别的皇后
    bool check(int x, int y, int dir)
    {
        bool flag = true;

        do
        {//一直检测直到到达边界

            x += m_dir[dir].x;//加上方向坐标
            y += m_dir[dir].y;

            flag  = (flag) && (m_chessboard[x][y] == 0);

        }while(flag);
        return (m_chessboard[x][y] == 2);//此时表示当到达边界后还是没有检测到皇后，所以该方向上确实没有皇后
    }

    void run(int j)
    {
        if(j <= SIZE)
        {
            for(int i = 1; i <= SIZE; i++)
            {
                if(check(i, j, 0) && check(i, j, 1) && check(i, j, 2))
                {
                    m_chessboard[i][j] = 1;
                    m_solution.insert(pos(i,j));

                    //递归：每行都能找到一个合适的位置的情况下叫做递归
                    run(j+1);

                    //回溯：如果在某一行找不到合适位置后就要进行回溯，
                    //根据函数调用栈的原理，当在第j+1行查找失败时就会回到调用结束的位置，
                    //然后再重新标记空间
                    m_chessboard[i][j] = 0;
                    m_solution.remove(m_solution.length() - 1);
                }
            }
        }
        else
        {
            m_count++;//变量表示方案的数目
            chess_print();
        }
    }

public:
    QueueQuestion()
    {
        chessinit();
    }

    void run()
    {
        cout << "start" << endl;
        run(1);
        cout << "Total:" <<m_count << endl;
    }

};

/*
    测试代码

    //递归实现链表逆序
    Node1* list =  create_list(1,5);
    print_list(list);
    list = reserve(list);
    print_list(list);
    destroy_list(list);

    cout << endl;
    //实现两个链表的合并，从小到大排序
    Node1* list1 =  create_list(1,5);
    Node1* list2 =  create_list(6,5);
    print_list(list1);
    print_list(list2);
    Node1* ret = merge(list1, list2);
    cout << endl;
    print_list(ret);
    destroy_list(ret);

    //汉诺塔
    HanoiTower(3, 'a', 'b', 'c');
    cout << endl;

    //不重复的全排列
    char s[] = "aac";
    permutation(s, s);//始终指向字符串首地址

    //逆序输出偶数节点
    cout << endl;
    Node1* list_re = create_list(1,9);
    print_list(list_re);
    re_print(list_re);
    destroy_list(list_re);



    //N皇后问题
    QueueQuestion<4> qq;

    qq.run();


    cout << "over" << endl;


*/

}

#endif // RECURSION_H
