#include <assert.h>
#include "Exceptions.h"



///////////////////////////////////////////  do-cosmetics ////////////////////////////////////////////////////////
/* 
    Node* left\right that is nulptr means subtree with hight=(-1)
 */
template <typename Key, typename Data>
class Node {
    Key key;
    Data data;
    Node* left;
    Node* right;

    int subTreeHight;

    // assuming Data and Key has copy ctor
    explicit Node(Key key, Data data, Node* left = nullptr, Node* right= nullptr, int subTreeHight = 0) 
        : key(key), data(data), left(left), right(right), subTreeHight(subTreeHight) {}    

    Data getData() {
        //assuming Data has copy ctor
        return data;
    }

    void setData(const Data& other_data) {
        //assuming Data has operator=()
        data = other_data;
    }

    Key getKey() {
        //assuming Key has copy ctor
        return key;
    }

    int getHight() {
        return subTreeHight;
    }

    void setHight(int hight) {
        subTreeHight = hight;
    }

};
///////////////////////////////////////////  do-cosmetics ////////////////////////////////////////////////////////






/* 
    Node* root that is nulptr means an empty tree
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


    //exceptions:

};


//assuming: Key has operators: < , > , =
template<typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::findNode(const Key& key, Node<Key,Data>* last){
        if( !last ){
            throw NullArgument();
        }

        last = root;
        Node<Key,Data>* current = root;
        
        while( current!=nullptr  &&  key!=current->key ){
            last=current;

            if( key < current->key ){
                current=current->left;
            }else if( key > current->key ){
                current=current->right;
            }
        }

        return current;
}

template<typename Key,typename Data>
Data* find(const Key& key){
    return &Data();
}

template<typename Key,typename Data>
bool insert(const Key& key, const Data& data){
    return false;
}


template<typename Key>
 bool remove(const Key& key){
     return true;
 }