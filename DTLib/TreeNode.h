#ifndef TREENODE_H
#define TREENODE_H
#include "Object.h"

namespace QSLib
{

template <typename T >
class TreeNode : public Object
{
protected:
    //在程序设计中，遇到多个地方会使用同一段代码时可以考虑进行代码封装或者实现代码重构，工程中尽量减少代码冗余
    //实现重构，将GTreeNode和BTreeNode都能够使用的代码重构到TreeNode中
    bool m_flag;
    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);
    void* operator new(unsigned int size) throw()//表示不抛出任何异常
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T>* parent;


    bool flag()
    {
        return m_flag;
    }

    TreeNode()
    {
        m_flag = false;
        parent = NULL;
    }

    virtual ~TreeNode() = 0;

};

template <typename T>
TreeNode<T>::~TreeNode()
{

}
}


#endif // TREENODE_H
