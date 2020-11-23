#include <assert.h>
#include "Exceptions.h"




/* 
    * general description: an auxiliary struct to represent a vertex in an avl tree.
    left\right that is nullptr means subtree with hight=(-1)

 */
template <typename Key, typename Data>
struct Node {
    const Key key;
    Data data;
    Node* left;
    Node* right;
    Node* parent;

    int subTreeHight;

    // assuming Data and Key has copy ctor
    explicit Node(Key key, Data data, Node* left = nullptr, Node* right= nullptr,
                  Node* parent=nullptr, int subTreeHight = 0) 
                : key(key), data(data), left(left), right(right), parent(parent) ,subTreeHight(subTreeHight) {}    
        
};



/* 
    root that is nullptr means an empty tree

 */
template  <typename Key,typename Data>
class AvlTree{
    
    Node<Key,Data>* root;

    /********* private methods: **********/
    AvlTree(Node<Key,Data>* root = nullptr) : root(root) {}
    AvlTree(const AvlTree& other) = delete;
    
    // returns the required node or null if failed. last <- last checked node
    Node<Key,Data>* findNode(const Key& key, Node<Key,Data>* last);

    public:
    Data* find(const Key& key);
    bool insert(const Key& key, const Data& data);
    bool remove(const Key& key);



};




//assuming: Key has operators: < , > , =
//last remains nullptr in case what we are looking for is the root itself
template<typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::findNode(const Key& key, Node<Key,Data>* last){
    last = nullptr;
    Node<Key,Data>* current = root;

    while( current!=nullptr  &&  key!=current->key() ){
        last=current;

        if( key < current->key() ){
            current=current->left;
        }else if( key > current->key() ){
            current=current->right;
        }
    }
    return current;
}



//returns null in case of failure
template<typename Key,typename Data>
Data* AvlTree<Key,Data>::find(const Key& key){
    Node<Key,Data>* lastOnSearch;
    Node<Key,Data>* desired = findNode(key,lastOnSearch);

    if(desired!=nullptr){
        return &(desired->data);
    }
    return nullptr;
}



template<typename Key,typename Data>
bool AvlTree<Key,Data>::insert(const Key& key, const Data& data){
    Node<Key,Data>* lastOnSearch;
    Node<Key,Data>* exists = findNode(key,lastOnSearch);
    if( exists ) return false;
    

    
}


template<typename Key,typename Data>
 bool AvlTree<Key,Data>::remove(const Key& key){
     return true;
 }