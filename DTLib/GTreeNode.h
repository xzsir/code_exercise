#ifndef GTREENODE_H
#define GTREENODE_H
#include "tree.h"
#include "LinkList.h"

namespace QSLib
{
template <typename T>
class GTreeNode : public TreeNode<T>
{

public:
    LinkList<GTreeNode<T>*> child;


    //实现工厂方法
    static GTreeNode<T>* NewNode()
    {
        GTreeNode<T>* ret = new GTreeNode<T>();
        if(ret != NULL)
        {
            ret->m_flag = true;
        }
        return ret;
    }

};

}



#endif // GTREENODE_H
