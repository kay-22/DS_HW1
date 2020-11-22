#include <assert.h>

/* 
    Node* left\right that is nulptr means subtree with hight=(-1)
 */
template <typename T>
class Node{
    T data;
    Node* left;
    Node* right;

    int subTreeHight;

};



/* 
    Node* root that is nulptr means an empty tree
 */
template  <typename T>
class AvlTree{
    Node<T>* root;
};