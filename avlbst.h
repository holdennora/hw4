#ifndef AVLBST_H
#define AVLBST_H
#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    int getHeight(AVLNode<Key, Value>* node) const;
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    bool isZigZig (AVLNode<Key, Value>* current);


};
//Gets subtree height
template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) const
{
  int height = -1;

  if(node != nullptr)
  {
    int leftHeight = -1;
    if (node->getLeft() != nullptr)
    {
      leftHeight = getHeight(node->getLeft());
    }

    int rightHeight = -1;
    if (node->getRight() != nullptr)
    {
       rightHeight = getHeight(node->getRight());
    }

    if (leftHeight > rightHeight)
    {
      height =  leftHeight +1;
    }
    else
    {
      height =  rightHeight +1;
    }
  }
  return height;
}
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
  AVLNode<Key, Value>* g = p->getParent();
  AVLNode<Key, Value>* parent = nullptr;

  bool isLeftChild = (g != nullptr && g->getLeft() == p);
  while(g != nullptr)
  {
    parent = g->getParent();
    int8_t balance = g->getBalance();

    if(g->getLeft() == p)
    {
      balance--;
    }
    else
    {
      balance++;
    }
    g->setBalance(balance);

    if(balance == 0)
    {
      break;
    }
    else if(balance == 2 || balance == -2)
    {
      if(balance == 2)
      {
        if(n->getKey() < p->getKey())
        {
          rotateRight(g);
        }
        else
        {
          rotateLeft(p);
          rotateRight(g);
        }
      }
      else
      {
        if(n->getKey() > p->getKey())
        {
          rotateLeft(g);
        }
        else
        {
          rotateRight(p);
          rotateLeft(g);
        }
      }
      break;
    }
      n = p;
      p = g;
      g = g->getParent();
  }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isZigZig (AVLNode<Key, Value>* current)
{
  AVLNode<Key, Value>* parent = current->getParent();
  AVLNode<Key, Value>* gParent = parent->getParent();

  if(gParent == nullptr)
  {
    return false;
  }
  if(parent == nullptr)
  {
    return false;
  }

  if(parent == gParent->getLeft())
  {
    if(current == parent->getLeft())
    {
      return true;
    }
  }
  else if(parent == gParent->getRight())
  {
     if(current == parent->getRight())
     {
      return true;
     }
  }
  return false;
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->root_ == nullptr)
    {
      AVLNode<Key, Value>* toInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      this->root_ = toInsert;
      toInsert->setBalance(0);
      return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(current != nullptr)
    {
      parent = current;
      if(new_item.first < current->getKey())
      {
        current = current->getLeft();
      }
      else if(new_item.first > current->getKey())
      {
        current = current->getRight();
      }
      else
      {
        current->setValue(new_item.second);
        return;
      }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(newNode->getKey() < parent->getKey())
    {
      parent->setLeft(newNode);
    }
    else
    {
      parent->setRight(newNode);
    }

    current = newNode;
    insertFix(parent, current);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  /*
    // TODO
    AVLNode<Key, Value>* nodetoRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(nodetoRemove == nullptr)
    {
      return;
    }
    AVLNode<Key, Value>* parent = nodetoRemove->getParent();
    AVLNode<Key, Value>* successor = nullptr;
    bool isLeftChild = false;

    if(parent != nullptr)
    {
      isLeftChild = (parent->getLeft() == nodetoRemove);
    }
    if(nodetoRemove->getLeft() == nullptr && nodetoRemove->getRight() == nullptr)
    {
      if(parent != nullptr)
      {
        if(isLeftChild)
        {
          parent->setLeft(nullptr);
        }
        else
        {
          parent->setRight(nullptr);
        }
      }
      else
      {
        this->root_ = nullptr;
      }
    delete nodetoRemove;
    }
    else if (nodetoRemove->getLeft() == nullptr)
    {
      if(parent != nullptr)
      {
        if(isLeftChild)
        {
          parent->setLeft(nodetoRemove->getRight());
        }
        else
        {
           parent->setRight(nodetoRemove->getRight());
        }
      }
      else
      {
        this->root_ = nodetoRemove->getLeft();
      }
      nodetoRemove->getLeft()->setParent(parent);
      delete nodetoRemove;
    }
    else
    {
      successor = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodetoRemove));
      nodeSwap(nodetoRemove, successor);
      remove(successor->getKey());
    }
    if(parent != nullptr)
    {
      AVLNode<Key, Value>* ancestor = parent;
      while(ancestor != nullptr)
      {
        insertFix(ancestor, nullptr);
        ancestor = ancestor->getParent();
      }
    }
    */
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
  bool found = false;

  AVLNode<Key, Value>* pivot = node->getRight();
  if(pivot == nullptr)
  {
    return;
  }
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* pivotLeft = pivot->getLeft();
  
  if(node == this->root_)
  {
    found = true;
  }
  pivot->setParent(parent);
  if (parent == nullptr)
  {
    this->root_ = pivot; 
  }
  else
  {
    if(node == parent->getLeft())
    {
      parent->setLeft(pivot);
    }
    else
    {
      parent->setRight(pivot);
    }
  }
  node->setRight(pivotLeft);
  if(pivotLeft != nullptr)
  {
    pivotLeft->setParent(node);
  }

  pivot->setLeft(node);
  node->setParent(pivot);

  if(found)
  {
    this->root_ = pivot;
  }

}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* child = node->getLeft();
  if(child == nullptr)
  {
    return;
  }
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* childR = child->getRight();
  
  child->setParent(parent);

  if (parent == nullptr)
  {
    this->root_ = static_cast<Node<Key, Value>*>(child); 
  }
  else
  {
    if(node == parent->getLeft())
    {
      parent->setLeft(child);
    }
    else
    {
      parent->setRight(child);
    }
  }
  node->setLeft(childR);
  if(childR != nullptr)
  {
    childR->setParent(node);
  }
  child->setRight(node);
  node->setParent(child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
