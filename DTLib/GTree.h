#ifndef GTREE_H
#define GTREE_H

#include "tree.h"
#include "GTreeNode.h"
#include "exception.h"
#include "LinkQueue.h"


namespace QSLib
{
template <typename T>
class GTree : public Tree<T>
{
protected:

    LinkQueue<GTreeNode<T>*> m_queue;


    //基于结点数据查找
    GTreeNode<T>* find(GTreeNode<T>* node, const T& value)const
    {
        GTreeNode<T>* ret = NULL;

        if(node != NULL)
        {
            if(node->value == value)
            {
                ret =  node;
            }
            else
            {
                for(node->child.move(0); (!node->child.end()) && (ret == NULL); node->child.next())
                {
                    ret = find(node->child.current(), value);
                }
            }

        }

        return ret;
    }
    //基于结点查找
    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj)const
    {
        GTreeNode<T>* ret = NULL;
        if(node != NULL)
        {
            if(node == obj)
            {
                return node;
            }
            else
            {
                for(node->child.move(0); (!node->child.end()) && (ret == NULL); node->child.next())
                {
                    ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }

    void free(GTreeNode<T>* node)
    {
        if(node != NULL)
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                free(node->child.current());
            }
            if(node->flag())
            {
                //在循环结束后，所有子树已经被释放，所以最后一步是释放根结点
                delete node;
            }

        }
    }

    void remove(GTreeNode<T>* node, GTree<T>*& ret)
    {
        ret = new GTree<T>();

        if(ret == NULL)
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree...");
        }
        else
        {
            if(root() == node)
            {
                this->m_root = NULL;//删除的是根结点时直接标记树为空
            }
            else
            {
                //创建一个引用指代node结点父节点的所有孩子结点，并保存在一个链表中。
                LinkList<GTreeNode<T>*>& child = dynamic_cast<GTreeNode<T>*>(node->parent)->child;
                child.remove(child.find(node));//remove为链表中的remove
                node->parent = NULL;//断开与父亲结点之间的关系
            }
            ret->m_root = node;//将删除的结点及其孩子结点保存在树对象ret中
        }
    }

    //获取结点数目
    int count(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            ret = 1;//当node不为空时表示至少可以设置为有一个结点了

            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                ret += count(node->child.current());
            }
        }

        return ret;
    }

    //获取树的高度
    int height(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int h = height(node->child.current());

                if(ret < h)
                {
                    ret = h;
                }
            }
            ret += 1;
        }

        return ret;
    }

    //获取树的度
    int degree(GTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != NULL)
        {
            ret = node->child.length();//获取根结点的度

            //遍历孩子，获取孩子的度，最后保存最大的度
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                int d = degree(node->child.current());

                if(ret < d)
                {
                    ret = d;
                }
            }
        }

        return ret;
    }


public:

    GTree()
    {

    }
    //以结点方式插入树中
    bool insert(TreeNode<T>* node)
    {
        bool ret = true;
        if( node != NULL)
        {
            if( this->m_root == NULL)
            {
                //如果当前根结点为NULL，表示此时为空树，将该结点作为根结点
                node->parent = NULL;
                this->m_root = node;//设置根结点
            }
            else
            {
               GTreeNode<T>* np = find(node->parent);
               if(np != NULL)
               {
                   GTreeNode<T>* n = dynamic_cast<GTreeNode<T>*>(node);
                   if(np->child.find(n) < 0)
                   {
                      np->child.insert(n);
                   }

               }
               else
               {
                   THROW_EXCEPTION(InvalidOperationException, "Invalid ope of prent to insert...");
               }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Invalid parameter to insert...");
        }

        return ret;
    }

    //给定一个父节点，通过数据来插入到树中
    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;

        GTreeNode<T>* node = GTreeNode<T>::NewNode();
        if(node != NULL)
        {
            node->value = value;
            node->parent = parent;


            insert(node);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, " No Enough Memory to Creat a New node...");
        }
        return ret;
    }


    //删除结点，基于结点数据删除
    SharedPointer< Tree<T> > remove(const T& value)
    {
        GTree<T>* ret = NULL;
        GTreeNode<T>* node = find(value);

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "Can not find a node...");
        }
        else
        {
            remove(node, ret);
            m_queue.clear();
        }
        return ret;
    }

    //基于结点来删除树中的结点
    SharedPointer< Tree<T> > remove(TreeNode<T>* node)
    {
        GTree<T>* ret = NULL;
        node = find(node);

        if(node == NULL)
        {
            THROW_EXCEPTION(InvalidParameterException, "para node is valid...");
        }
        else
        {
            remove(dynamic_cast<GTreeNode<T>*>(node), ret);
            m_queue.clear();
        }
        return ret;
    }

    //基于结点数据查找结点，返回结点指针
    GTreeNode<T>* find(const T& value)const
    {
        return find(root(), value);
    }

    //基于结点查找，返回结点指针
    GTreeNode<T>* find(TreeNode<T>* node)const
    {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
    }

    //获取根结点指针
    GTreeNode<T>* root()const
    {
        return dynamic_cast<GTreeNode<T>*>(this->m_root);
    }


    //获取树的度
    int degree() const
    {
        return degree(root());
    }

    //获取树中的结点数
    int count() const
    {
        return count(root());
    }

    //获取树的高度
    int height() const
    {
        return height(root());
    }

    //清空树
    void clear()
    {
        free(root());
        this->m_root = NULL;
        m_queue.clear();
    }

    //遍历：
    bool begin()
    {
        bool ret = (root() != NULL);

        if(ret)
        {
            m_queue.clear();
            m_queue.add(root());
        }

        return ret;
    }

    bool end()
    {
        return (m_queue.length() == 0);
    }

    bool next()
    {
        bool ret = (m_queue.length() > 0);

        if(ret)
        {
             //取出队头元素
            GTreeNode<T>* node = m_queue.front();
            //清除对头元素，相当于移动游标
            m_queue.remove();
            //将node结点的孩子全部压入队列
            for(node->child.move(0); !node->child.end(); node->child.next())
            {
                m_queue.add(node->child.current());
            }
        }

        return ret;
    }

    T current()
    {
        if(!end())
        {
            return m_queue.front()->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
        }
    }





    ~GTree()
    {
        clear();
    }
};

}

#endif // GTREE_H
