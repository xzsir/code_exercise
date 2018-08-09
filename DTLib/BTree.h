#ifndef BTREE_H
#define BTREE_H
#include "exception.h"
#include "Tree.h"
#include "BTreeNode.h"
#include "sharedpointer.h"

namespace QSLib
{
template <typename T >
class BTree : public Tree<T>
{
protected:

    //基于数据元素查找，函数为const函数
    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value)const
    {
        BTreeNode<T>* ret = NULL;

        if(node != NULL)
        {
            if(node->value == value)
            {
                ret = node;
            }
            else
            {
                if(ret == NULL)
                {
                    ret = find(node->left, value);
                }
                if(ret == NULL)
                {
                    ret = find(node->right, value);
                }
            }
        }
        return ret;
    }

    //基于结点查找，函数是const版本
    virtual BTreeNode<T>* find(BTreeNode<T>* node, BTreeNode<T>* obj)const
    {
        BTreeNode<T>* ret = NULL;

        if( (node != NULL) && (obj != NULL) )
        {
            if(node == obj)
            {
                ret = node;
            }
            else
            {
                if(ret == NULL)
                {
                    ret = find(node->left, obj);
                }
                if(ret == NULL)
                {
                    ret = find(node->right, obj);
                }
            }
        }
        return ret;
    }

    //添加辅助insert函数，，函数功能：将结点n插入到父结点np的pos位置处
    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;

        if(pos == ANY)
        {
            if(np->left == NULL){ np->left = n; }
            else if(np->right == NULL){ np->right = n; }
            else{ ret = false; }
        }
        else if(pos == LEFT)
        {
            if( np->left == NULL ){ np->left = n; }
            else{ ret = false; }
        }
        else if(pos == RIGHT)
        {
            if(np->right == NULL){ np->right = n; }
            else{ ret = false; }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();   //申请一片堆空间保存删除结点所在的子树

        if(ret == NULL)
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory create a tree");
        }
        else
        {
            if( node == this->m_root  )//删除的是根结点
            {
                this->m_root = NULL;//切断联系
                ret->m_root = node;//将node所在的树保存到堆空间的ret
            }
            else
            {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);//node->parent是TreeNode类型
                //处理左子树或右子树
                if(parent->left == node)
                {
                    parent->left = NULL;
                }
                else if(parent->right == node)
                {
                    parent->right = NULL;
                }

                node->parent = NULL;//断开node结点的父亲结点
                ret->m_root = node;
            }
        }

    }

    //功能函数，释放结点
    virtual void free(BTreeNode<T>* node)
    {
        if(node != NULL)
        {
            free(node->left);
            free(node->right);
            std::cout << node->value << std::endl;
            if( node->flag() )
            {
                delete node;
            }
        }
    }


    //获取结点数目
    int count(BTreeNode<T>* node) const
    {
        int ret = 0;

        //可以用三目运算符改写
        if(node != NULL)
        {
            ret = 1;//当node不为空时表示至少可以设置为有一个结点了
            ret += count(node->left);
            ret += count(node->right);
        }

        return ret;
    }


    //获取树的高度
    int height(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            int h1 = height(node->left);
            int h2 = height(node->right);

            ret = (h1 < h2) ? h2 : h1;

            ret += 1;
        }

        return ret;
    }

    //获取树的度
    int degree(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {

            /*
             //方法一
            if( (node->left != NULL) && (node->right != NULL) )
            {
                ret = 2;
            }
            else if( (node->left != NULL) || (node->right != NULL) )
            {
                int d1 =degree(node->left);
                if( ret < d1 )
                {
                    ret = d1;
                }
                int d2 = degree(node->right);
                if(ret < d2)
                {
                    ret = d2;
                }
            }
            */

            /*
            //方法二
            int dl = degree(node->left);
            int dr = degree(node->right);

            ret = !!(node->left) + !!(node->right);
            ret = ( (dl > dr) ? dl : dr ) > ret ? ( (dl > dr) ? dl : dr ) : ret;
            */

            //方法三

            BTreeNode<T>* child[] = {node->left, node->right};
            ret = !!(node->left) + !!(node->right);

            for(int i = 0; i < 2 && (ret <2); i++)
            {
                int d = degree(child[i]);
                if(ret < d)
                {
                    ret = d;
                }
            }
        }

        return ret;
    }

public:

    //基于结点插入
    virtual bool insert(TreeNode<T>* node, BTNodePos pos)
    {
        bool ret = true;
        if(node != NULL)
        {
            if(this->m_root== NULL)//如果根结点为空，设置为空结点
            {
                node->parent = NULL;
                this->m_root = node;
            }
            else
            {
               BTreeNode<T>* np = dynamic_cast<BTreeNode<T>*>( find(node->parent) );//查找待插入结点的父亲结点是否存在
               if(np != NULL)
               {
                    ret = insert(dynamic_cast<BTreeNode<T>*>( node ), np, pos);//最后的插入操作
               }
               else
               {
                   THROW_EXCEPTION(InvalidParameterException, "Invalid parent tree node...");
               }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Invalid para tree node...");
        }
        return ret;
    }

    bool insert(TreeNode<T>* node)
    {
       return insert(node, ANY);
    }




    //基于数据插入
    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
    {
        bool ret = true;
        BTreeNode<T>* node = BTreeNode<T>::NewNode();//工厂模式创建新结点
        if(node != NULL)
        {
            node->value = value;//设置结点属性
            node->parent = parent;

            ret = insert(node, pos);//将结点插入到pos位置
            if(ret != true)
            {
                delete node;//插入失败销毁结点
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to new a node...");
        }
        return ret;
    }

   bool insert(const T& value, TreeNode<T>* parent)
   {
       return insert(value, parent, ANY);
   }




   //删除操作  函数返回类型是SharedPointer< Tree<T> >   实际返回类型是 BTree<T>*
   //ret在remove(node, ret);调用后指向的是堆空间的内存，相当于 remove(const T& value)函数返回的是堆空间中的一棵树
   SharedPointer< Tree<T> > remove(const T& value)
   {
       BTree<T>* ret = NULL;//定义返回值变量

       BTreeNode<T>* node = find(value);//查找待删除的结点是否存在
       if(node != NULL)
       {
            remove(node, ret);//删除操作
            node->parent = NULL;
       }
       else
       {
           THROW_EXCEPTION(InvalidParameterException, "no node in tree via value...");
       }

       return ret;
   }
   SharedPointer< Tree<T> > remove(TreeNode<T>* node)
   {
       BTree<T>* ret = NULL;

       node = find(node);

       if(node != NULL)
       {
            remove(dynamic_cast<BTreeNode<T>*>(node), ret);//类型处理，传入的参数是TreeNode类型
            node->parent = NULL;
       }
       else
       {
           THROW_EXCEPTION(InvalidParameterException, "no node in tree via value...");
       }

       return ret;
   }


   //BTreeNode<T>* 返回类型一定是从TreeNode中继承来的
   BTreeNode<T>* find(const T& value)const
   {
       return find(root(), value );
   }
   BTreeNode<T>* find(TreeNode<T>* node)const
   {
       return find(root(), dynamic_cast<BTreeNode<T>*>(node));
   }



   BTreeNode<T>* root()const
   {
       return dynamic_cast<BTreeNode<T>*>(this->m_root);
   }



   int degree() const
   {
       return degree(root());
   }

   int count() const
   {
       return count(root());
   }

   int height() const
   {
       return height(root());
   }

   void clear()
   {
       free(root());
       this->m_root = NULL;
   }
   ~BTree()
   {
       clear();

   }


};

}


#endif // BTREE_H
