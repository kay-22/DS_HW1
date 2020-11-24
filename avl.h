#include <assert.h>
#include "Exceptions.h"




/* 
    * general description: an auxiliary struct to represent a vertex in an avl tree.
    * left\right that is nullptr means subtree with hight=(-1)

 */
template <typename Key, typename Data>
struct Node {
    const Key key;
    Data data;
    Node* parent;
    Node* left;
    Node* right;

    
    int subTreeHight;

    // assuming Data and Key has copy ctor
    explicit Node(Key key, Data data, Node* parent=nullptr, Node* left = nullptr, Node* right= nullptr,
                  int subTreeHight = 0) 
                : key(key), data(data), left(left), right(right), parent(parent) ,subTreeHight(subTreeHight) {}    
        
};




/* 
    root that is nullptr means an empty tree

 */
template  <typename Key,typename Data>
class AvlTree{

    public:
    explicit AvlTree(Node<Key,Data>* root = nullptr);
    Data* find(const Key& key);
    bool insert(const Key& key, const Data& data);
    bool remove(const Key& key);
    
    
    // Non-interface part:
    ~AvlTree(); //   <---TODO
    private:
    Node<Key,Data>* root;

    AvlTree(const AvlTree& other) = delete;
    // returns the required node or null if failed. last <- last checked node
    Node<Key,Data>* findNode(const Key& key, Node<Key,Data>* last);
    // inserts to the AvlTree like BinaryTreeInsert. assuming not-exists.
    Node<Key,Data>* binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last );
    // assures that the balanceFactor of the node is correct by applying "role-over" if neccessary.
    void assureBalance(Node<Key,Data>* nodeOnTrack);

};


//by default: create an empty new AvlTree
template  <typename Key,typename Data>
AvlTree<Key,Data>::AvlTree(Node<Key,Data>* root) : root(root) {}


//assuming: Key,Data has operators: < , > , =
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


//assuming: Key,Data has copy Ctor and operator=
//assuming: Key,Data has operators: < , > , =
template  <typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last ){
    Node<Key,Data>* newNode = new Node<Key,Data>(key,data,last);
    
    if( last==nullptr ){//meaning an empty tree
        assert( this->root==nullptr );
        root = newNode;
    }
    else if( key < last->key ){
        last->left = newNode;
    }
    else{ assert( key > last->key );
        last->right = newNode;
    }
    return newNode;
}


//assuming: Key,Data has copy Ctor and operator=
//assuming: Key,Data has operators: < , > , =
template  <typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last ){
    Node<Key,Data>* newNode = new Node<Key,Data>(key,data,last);
    
    if( last==nullptr ){//meaning an empty tree
        assert( this->root==nullptr );
        root = newNode;
    }
    else if( key < last->key ){
        last->left = newNode;
    }
    else{ assert( key > last->key );
        last->right = newNode;
    }
    return newNode;
}


template  <typename Key,typename Data>
void AvlTree<Key,Data>::assureBalance(Node<Key,Data>* nodeOnTrack){
    //todo
    return;
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



//return values: True for "sucssess", False for  "already exists".
template<typename Key,typename Data>
bool AvlTree<Key,Data>::insert(const Key& key, const Data& data){
    Node<Key,Data>* lastOnSearch;
    Node<Key,Data>* exists = findNode(key,lastOnSearch);
    
    if( exists ) return false;
    assert(lastOnSearch);
    Node<Key,Data>* nodeOnTrack = binaryInsert(key,data,lastOnSearch);
    
    do{
        assureBalance(nodeOnTrack);//  <--should also check suc
        nodeOnTrack = nodeOnTrack->parent;
    }while ( nodeOnTrack->parent );

    return true;   
}




//return values: True for "sucssess", False for  "not exists".
template<typename Key,typename Data>
 bool AvlTree<Key,Data>::remove(const Key& key){
    Node<Key,Data>* lastOnSearch;
    Node<Key,Data>* exists = findNode(key,lastOnSearch);

    if( !exists ) return false;
    assert(lastOnSearch);
    Node<Key,Data>* nodeOnTrack = binaryRemove(key,data,lastOnSearch);
    //todo - co,plete the function

     return true;
 }