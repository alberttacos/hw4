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
    void corinsert(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void Rrotate(AVLNode<Key, Value>* location);
    void Lrotate(AVLNode<Key, Value>* location);
    void corremove(AVLNode<Key, Value>* node, int differ);
    void AVLNodedeleter(AVLNode<Key,Value>* target);
    void insertBST(const std::pair<const Key, Value> &keyValuePair);


};

template<class Key, class Value>
void AVLTree<Key,Value>::insertBST(const std::pair<const Key, Value> &keyValuePair){
  AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->root_);
  if(target == NULL){
    this->root_ = new AVLNode<Key,Value>(keyValuePair.first , keyValuePair.second , NULL);
    return;
  }

  while (target != NULL){
    if(keyValuePair.first < target->getKey()){
      if(target->getLeft()){
        target = target->getLeft();
      }
      else{
        AVLNode<Key, Value>* newnode = new AVLNode<Key,Value>(keyValuePair.first, keyValuePair.second, target);
        target->setLeft(newnode);
        break;
      }
    }
    else if(keyValuePair.first > target ->getKey()){
      if(target->getRight()){
        target = target->getRight();
        }
        else{
          AVLNode<Key, Value>* newnode = new AVLNode<Key,Value>(keyValuePair.first, keyValuePair.second, target);
          target->setRight(newnode);
          break;
        }
    }
    else if(keyValuePair.first == target->getKey()){
      target->setValue(keyValuePair.second);
      break;
    }
  }
}
template<class Key, class Value>
void AVLTree<Key,Value>::AVLNodedeleter(AVLNode<Key,Value>* target){
  if(target->getParent()){
    if(target->getParent()->getRight() == target){
      target->getParent()->setRight( NULL );
    }
    else if (target->getParent()->getLeft() == target){ 
      target->getParent()->setLeft(NULL);
    }
    delete target;
  }
  else{
    delete target;
    this->root_= NULL;
  }
}
template<class Key, class Value>
void AVLTree<Key,Value>::corinsert(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node){
  if(parent == NULL || parent->getParent() == NULL){
    return;
  }
  AVLNode<Key, Value>* Grandparent = parent->getParent();

  if( parent == Grandparent->getLeft()){
    Grandparent->updateBalance(-1);
    if(Grandparent->getBalance() == 0){
      return;
    }
    else if(Grandparent->getBalance() == -1){
      corinsert(Grandparent, parent);
    }else if(Grandparent->getBalance() == -2){
    if(node == parent ->getLeft()){
      Rrotate(Grandparent);
      parent->setBalance(0);
      Grandparent->setBalance(0);
    }
    else if(parent->getRight() == node){
      Lrotate(parent);
      Rrotate(Grandparent);

      if(node->getBalance() == -1){
        parent->setBalance(0);
        Grandparent->setBalance(1);
        node->setBalance(0);
      }
      else if(node->getBalance() == 0){
        parent->setBalance(0);
        Grandparent->setBalance(0);
        node->setBalance(0);
      } 
      else if(node->getBalance() == 1){
        parent->setBalance(-1);
        Grandparent->setBalance(0);
        node->setBalance(0);
      }
      
    }
    }
  }
  else if(Grandparent->getRight() == parent){
    Grandparent->updateBalance(1);
    if(Grandparent->getBalance() == 0){
      return;
    }
    if(Grandparent->getBalance() == 1){
      corinsert(Grandparent, parent);
    }else if(Grandparent->getBalance() == 2){
    if (node == parent->getRight()){
      Lrotate(Grandparent);
      parent->setBalance(0);
      Grandparent->setBalance(0);
    }
    else if(parent->getLeft() == node){
      Rrotate(parent);
      Lrotate(Grandparent);

      if(node->getBalance() == 1){
        parent->setBalance(0);
        Grandparent->setBalance(-1);
        node->setBalance(0);
      }
      else if(node->getBalance() == 0){
        parent->setBalance(0);
        Grandparent->setBalance(0);
        node->setBalance(0);
      }
      else if(node->getBalance() == -1){
        parent->setBalance(1);
        Grandparent->setBalance(0);
        node->setBalance(0);
      }
      
    }
  }
  }
}

template<class Key, class Value>
void AVLTree<Key,Value>::Rrotate(AVLNode<Key, Value>* location){

    AVLNode<Key,Value>* childrenb = location->getLeft();
    AVLNode<Key,Value>* parent = location->getParent();
    AVLNode<Key,Value>* tempnode = childrenb->getRight();

    if(location == this->root_){
      this->root_ = childrenb;
    }    
    childrenb->setRight(location);
    location->setParent(childrenb);
    childrenb->setParent(parent);

    if(parent){
      if(parent->getRight() == location){
        parent ->setRight(childrenb);
      }
      else if(parent->getLeft() == location){
        parent->setLeft(childrenb);
      }
    }
    location->setLeft(tempnode);
    if(tempnode){
      tempnode->setParent(location);
    }
}
    
template<class Key, class Value>
void AVLTree<Key,Value>::Lrotate(AVLNode<Key, Value>* location){

   AVLNode<Key,Value>*  childrenb = location->getRight();
    AVLNode<Key,Value>* parent = location->getParent();
    AVLNode<Key,Value>* tempnode = childrenb->getLeft();

    if(location == this->root_){
      this->root_ = childrenb;
    }    
    childrenb->setLeft(location);
    location->setParent(childrenb);
    childrenb->setParent(parent);

    if(parent){
      if(parent->getRight() == location){
        parent ->setRight(childrenb);
      }
      else if(parent->getLeft() == location){
        parent->setLeft(childrenb);
      }
    }
    location->setRight(tempnode);
    if(tempnode){
      tempnode->setParent(location);
    }
}

template<class Key, class Value>
void AVLTree<Key,Value>::corremove(AVLNode<Key, Value>* node, int differ){
    if(node == NULL){
      return;
    }

    AVLNode<Key,Value>* parent = node->getParent();
     
    int removaldiff = 0;

    if(parent){
      if(parent->getLeft() == node){
        removaldiff = 1;
      } else if(parent->getRight() == node){
        removaldiff = -1;
      }
    }

    if (differ ==-1){
        if (differ + node->getBalance()==-2){
            AVLNode<Key,Value>* children=node->getLeft();
            if (children->getBalance()==-1){
                Rrotate(node);
                node->setBalance(0);
                children->setBalance(0);
                corremove(parent,removaldiff);
            }
            else if (children->getBalance()==0){
                Rrotate(node);
                node->setBalance(-1);
                children->setBalance(1);
            }
            else if (children->getBalance()==1){
                AVLNode<Key,Value>* grandchild=children->getRight();
                Lrotate(children);
                Rrotate(node);
                if (grandchild->getBalance()==1){
                    node->setBalance(0);
                    children->setBalance(-1);
                    grandchild->setBalance(0);
                }
                else if (grandchild->getBalance()==0){
                    node->setBalance(0);
                    children->setBalance(0);
                    grandchild->setBalance(0);
                }
                else if (grandchild->getBalance()==-1){
                    node->setBalance(1);
                    children->setBalance(0);
                    grandchild->setBalance(0);
                }
                corremove(parent, removaldiff);
            }
        }
        else if (differ + node->getBalance()==-1){
            node->setBalance(-1);
        }
        else if (differ + node->getBalance()==0){
            node->setBalance(0);
            corremove(parent, removaldiff);
        }
    }
    else if (differ ==1){
        if (differ + node->getBalance()==2){
            AVLNode<Key,Value>* children=node->getRight();//
            if (children->getBalance()==1){
                Lrotate(node);
                node->setBalance(0);
                children->setBalance(0);
                corremove(parent,removaldiff);
            }
            else if (children->getBalance()==0){
                Lrotate(node);
                node->setBalance(1);
                children->setBalance(-1);
            }
            else if (children->getBalance()==-1){
                AVLNode<Key,Value>* grandchild =children->getLeft();
                Rrotate(children);
                Lrotate(node);
                if (grandchild->getBalance()==-1){
                    node->setBalance(0);
                    children->setBalance(1);
                    grandchild->setBalance(0);
                }
                else if (grandchild->getBalance()==0){
                    node->setBalance(0);
                    children->setBalance(0);
                    grandchild->setBalance(0);
                }
                else if (grandchild->getBalance()==1){
                    node->setBalance(-1);
                    children->setBalance(0);
                    grandchild->setBalance(0);
                }
                corremove(parent, removaldiff);
            }
        }
        else if (differ + node->getBalance()==1){
            node->setBalance(1);
        }
        else if (differ + node->getBalance()==0){
            node->setBalance(0);
            corremove(parent, removaldiff);
        }
    }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(BinarySearchTree<Key,Value>::empty()){
    AVLNode<Key, Value>* new_root = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    this->root_ = new_root;
    new_root->setBalance(0);
    }
    else{
      insertBST(new_item);
      AVLNode<Key,Value>* new_item2 = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
      new_item2->setBalance(0);

      if(new_item2->getParent()){
        if(new_item2->getParent()->getBalance() == -1){
          new_item2->getParent()->setBalance(0);
        }
        else if(new_item2->getParent()->getBalance() == 1){
          new_item2->getParent()->setBalance(0);
        }
        else if(new_item2->getParent()->getBalance() == 0){
          if(new_item2->getParent()->getLeft() == new_item2){
            new_item2->getParent()->setBalance(-1);
            corinsert(new_item2->getParent(), new_item2);
          }
          else if(new_item2->getParent()->getRight() == new_item2){
            new_item2->getParent()->setBalance(1);
            corinsert(new_item2->getParent(), new_item2);
          }
        }
      }
    }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key,Value>* target = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::internalFind(key));
    int diff = 0;
    if(target == NULL){
        return;
    }
    while(target->getLeft() && target->getRight()){
        AVLNode<Key, Value>* predec = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key,Value>::predecessor(target));
        this->nodeSwap(predec, target);
    }
    AVLNode<Key, Value>* parent = target->getParent();
    if(parent != NULL){
      if(target == parent->getLeft()){
        diff = 1;
      }
      if(target == parent->getRight()){
         diff = -1;
      }
      }
      if(target->getRight() == NULL && target->getLeft() == NULL){
        this->AVLNodedeleter(target);
      }
      else if(target->getLeft()){
        AVLNode<Key,Value>* new_root = target->getLeft();
        if(target->getParent()){
          new_root->setParent(target->getParent());
          if(target->getParent()->getRight() == target){
            target->getParent()->setRight(new_root);
          }
          else if(target->getParent()->getLeft() == target){
            target->getParent()->setLeft(new_root);
          }
        }
        else{
          new_root->setParent(NULL);
          this->root_ = new_root;

        }
        delete target;
      }
      else if(target->getRight()){
        AVLNode<Key,Value>* new_root = target->getRight();
        if(target->getParent()){
          new_root->setParent(target->getParent());
          if(target->getParent()->getRight() == target){
            target->getParent()->setRight(new_root);
          }
          else if(target->getParent()->getLeft() == target){
            target->getParent()->setLeft(new_root);
          }
        }
        else{
          new_root->setParent(NULL);
          this->root_ = new_root;
        }
        delete target;
      }
      corremove(parent, diff);
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
