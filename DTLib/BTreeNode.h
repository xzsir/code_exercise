#ifndef BTREENODE_H
#define BTREENODE_H

#include "treenode.h"



namespace QSLib
{

enum BTNodePos
{
    ANY,
    LEFT,
    RIGHT
};


template <typename T>
class BTreeNode : public TreeNode<T>
{

public:
  BTreeNode<T>* left;
  BTreeNode<T>* right;
  BTreeNode()
  {
      left = NULL;
      right = NULL;
  }

  //实现工厂方法
  static BTreeNode<T>* NewNode()
  {
      BTreeNode<T>* ret = new BTreeNode<T>();
      if(ret != NULL)
      {
          ret->m_flag = true;
      }
      return ret;
  }


};

}


#endif // BTREENODE_H
