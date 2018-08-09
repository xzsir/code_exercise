#ifndef TREE_H
#define TREE_H

#include "sharedpointer.h"
#include "TreeNode.h"
#include "Object.h"

//虚函数重写返回值的设定应该是从纯虚函数的虚函数的返回值中派生而来
//如果虚函数的重写返回值不同时：一般正常的设计是，给返回值类型也定义一个基类，这样所有的返回值类型都从这个基类派生，返回值类型设置成这个派生类的指针

namespace QSLib {

template < typename T>
class Tree : public Object
{
protected:
    TreeNode<T>* m_root;
    Tree(const Tree<T>& obj);//禁止树之间赋值
    Tree<T>& operator = (const Tree<T>& obj);

public:
    Tree() { m_root = NULL; }
    virtual bool insert(TreeNode<T>* node) = 0;
    virtual bool insert(const T& value, TreeNode<T>* parent) = 0;

    virtual SharedPointer< Tree<T> > remove(const T& value) = 0;
    virtual SharedPointer< Tree<T> > remove(TreeNode<T>* node) = 0;

    virtual TreeNode<T>* find(const T& value)const = 0;
    virtual TreeNode<T>* find(TreeNode<T>* node)const = 0;

    virtual TreeNode<T>* root()const = 0;

    virtual int degree() const = 0;

    virtual int count() const = 0;

    virtual int height() const = 0;

    virtual void clear() = 0;

};
}

#endif // TREE_H
