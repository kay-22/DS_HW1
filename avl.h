#include <assert.h>
#include <iostream>
#include "Exceptions.h"

static const int GO_RIGHT = -2;
static const int GO_LEFT = 2;
static const int  NO_HIGHT = -1;

template <typename Key, typename Data>
struct Node;

//static functions declerations:

    template  <typename Key,typename Data>
    static inline Node<Key,Data>* findSuccessor(Node<Key,Data>* deleted);

    template  <typename Key,typename Data>
    static inline void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u);

    template  <typename T>
    static inline void swap(T& t1, T& t2);

    template  <typename Key,typename Data>
    static inline void removeLeaf(Node<Key,Data>* deleted, Node<Key,Data>** deltedSrc);

    template  <typename Key,typename Data>
    static inline void removeVertexWithOneSon(Node<Key,Data>* deleted, Node<Key,Data>** deltedSrc);

    template  <typename Key,typename Data,typename Functor>
    static void postOrderScan(Node<Key,Data>* root, const Functor& handle);// what happens when handle does not take Node type? <-------------------------------------------------------

    template  <typename Key,typename Data>
    static inline int balanceOf(Node<Key,Data>* nodeOnTrack);

    // template<typename Key,typename Data>
    // struct deleteNodeFunc;
    

//end declerations


/* 
    * general description: an auxiliary struct to represent a vertex in an avl tree.
    * left\right that is nullptr means subtree with hight=(-1)

 */
template <typename Key, typename Data>
struct Node {
    Key key;
    Data data;
    Node* parent;
    Node* left;
    Node* right;

    
    int subTreeHight;

    // assuming Data and Key has copy ctor
    explicit Node(const Key& key, const Data& data, Node* parent=nullptr, Node* left = nullptr,
                   Node* right= nullptr, int subTreeHight = 0) 
                  : key(key), data(data), parent(parent), left(left), right(right) ,subTreeHight(subTreeHight) {}     
};




/* 
    root that is nullptr means an empty tree

 */
template  <typename Key,typename Data>
class AvlTree{
//to-do: check if need const in functions
    public:
    explicit AvlTree(Node<Key,Data>* root = nullptr);
    Data*    find(const Key& key);
    bool     insert(const Key& key, const Data& data);
    bool     remove(const Key& key);
    void     clear();
    
    
    // Non-interface part:
    ~AvlTree();
    AvlTree& operator=(const AvlTree& other) = delete;
    private:
    Node<Key,Data>* root;

    //AvlTree(const AvlTree& other) = delete;

    /* returns the required node or null if failed. last <- last checked node */
    Node<Key,Data>* findNode(const Key& key, Node<Key,Data>*& last);
    /* inserts to the AvlTree like BinaryTreeInsert. assuming "not-exists". returns a poiner to the added node. */
    Node<Key,Data>* binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last );
    /* removes from the AvlTree like BinaryTreeRemove. assuming "exists". returns a pointr to the lowest on track. */
    Node<Key,Data>* binaryRemove(Node<Key,Data>* deleted);
    /* assures that the hight is updated after removal/insertion */
    void assureHight(Node<Key,Data>* nodeOnTrack);
    /* assures that the balanceFactor of the node is correct by applying "role-over" if neccessary. */
    void assureBalance(Node<Key,Data>* nodeOnTrack);
    /* performs a simple rotation to the left/right -
       where pivot is the either the unbalanced vertex or the son of the unbalanced */
    void rotateLeft(Node<Key,Data>* pivotSon);
    void rotateRight(Node<Key,Data>* pivotSon);

    friend std::ostream& operator<<(std::ostream& os, const AvlTree<Key,Data>& tree){
        struct PrintData{
            void operator()(Node<Key,Data>* node) const{
                std::cout << node->data << " " << balanceOf(node) << " " << node->subTreeHight << std::endl;
            }
        };
        postOrderScan(tree.root, PrintData());

        return os;
}

};

//by default: create an empty new AvlTree
template  <typename Key,typename Data>
AvlTree<Key,Data>::AvlTree(Node<Key,Data>* root) : root(root) {}


//assuming: Key,Data has operators: < , > , =
//last remains nullptr in case what we are looking for is the root itself
template<typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::findNode(const Key& key, Node<Key,Data>*& last){
    last = nullptr;
    Node<Key,Data>* current = root;

    while( current!=nullptr  &&  key!=current->key ){
        last=current;

        if( key < current->key ){
            current=current->left;
        }else if( key > current->key){
            current=current->right;
        }
    }
    return current;
}

//assuming that deleted has 2 sons
template  <typename Key,typename Data>
static inline Node<Key,Data>* findSuccessor(Node<Key,Data>* predecessor){
    assert(predecessor->right && predecessor->left);
    
    Node<Key,Data>* currentNode = predecessor->right;
    while (currentNode->left) {
        currentNode = currentNode->left;
    }

    assert(currentNode->key > predecessor->key);
    return currentNode;
}

template  <typename T>
static inline void swap(T& t1, T& t2){  
    T temp;

    temp = t1;
    t1 = t2;
    t2 = temp;
}

template  <typename Key,typename Data>
static inline void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u){
    swap<Key>(v->key,u->key);    
    swap<Data>(v->data,u->data);    
}


//assuming: Key,Data has copy Ctor and operator=
//assuming: Key,Data has operators: < , > , =
template  <typename Key,typename Data>
Node<Key,Data>* AvlTree<Key,Data>::binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last ){
    Node<Key,Data>* newNode = new Node<Key,Data>(key,data,last);
    
    if( last==nullptr ){//meaning an empty tree
        assert( this->root==nullptr );
        root = newNode;
        return newNode;
    }

    assert( (last->left == nullptr) || (last->right == nullptr));
    if( key < last->key ){
        last->left = newNode;
    }
    else{ assert( key > last->key );
        last->right = newNode;
    }

    return newNode;

}

template<typename Key, typename Data>
static inline void removeLeaf(Node<Key,Data>* deleted, Node<Key,Data>** deletedSrc){
    *deletedSrc = nullptr;
}


template<typename Key, typename Data>
static inline void removeVertexWithOneSon(Node<Key,Data>* deleted, Node<Key,Data>** deletedSrc){
    assert(*deletedSrc != nullptr);

    Node<Key,Data>* deletedSon;
    if (deleted->left) {
        deletedSon = deleted->left;
    }
    else { assert(deleted->right);
        deletedSon = deleted->right;
    }

    *deletedSrc = deletedSon;
    deletedSon->parent = deleted->parent;
}


template<typename Key, typename Data>
void AvlTree<Key,Data>::rotateRight(Node<Key,Data>* pivotSon){

    assert( pivotSon->parent );
    Node<Key,Data>* oldFather = pivotSon->parent;
    assert( oldFather->left==pivotSon );

    Node<Key,Data>* oldGrandFather = pivotSon->parent->parent;
    if( oldGrandFather==nullptr ) { ////////////////////////////////parent is the root
        root = pivotSon;
    }else if( oldGrandFather->right==pivotSon->parent ){///////////////parent is a rightSon
        oldGrandFather->right = pivotSon;
    }else{ assert( oldGrandFather->left==pivotSon->parent ); //////////parent is a leftSon
        oldGrandFather->left = pivotSon;
    }
    pivotSon->parent = oldGrandFather;


    oldFather->left = pivotSon->right;
    if( pivotSon->right!=nullptr ) pivotSon->right->parent = oldFather;
    pivotSon->right = oldFather;
    oldFather->parent = pivotSon;

    assureHight(pivotSon);
    assureHight(pivotSon->right);
}
    


template<typename Key, typename Data>
void AvlTree<Key,Data>::rotateLeft(Node<Key,Data>* pivotSon){

    assert( pivotSon->parent );
    Node<Key,Data>* oldFather = pivotSon->parent;
    assert( oldFather->right==pivotSon );

    Node<Key,Data>* oldGrandFather = pivotSon->parent->parent;
    if( oldGrandFather==nullptr ) { ////////////////////////////////parent is the root
        root = pivotSon;
    }else if( oldGrandFather->right==pivotSon->parent ){///////////////parent is a rightSon
        oldGrandFather->right = pivotSon;
    }else{ assert( oldGrandFather->left==pivotSon->parent ); //////////parent is a leftSon
        oldGrandFather->left = pivotSon;
    }
    pivotSon->parent = oldGrandFather;


    oldFather->right = pivotSon->left;
    if( pivotSon->left!=nullptr ) pivotSon->left->parent = oldFather;
    pivotSon->left = oldFather;
    oldFather->parent = pivotSon;

    assureHight(pivotSon);
    assureHight(pivotSon->left);
}
    



template  <typename Key,typename Data>
static inline int balanceOf(Node<Key,Data>* nodeOnTrack){
    int leftHight = (nodeOnTrack->left)? nodeOnTrack->left->subTreeHight : NO_HIGHT;
    int rightHight = (nodeOnTrack->right)? nodeOnTrack->right->subTreeHight : NO_HIGHT;

    return leftHight - rightHight;
}





template <typename Key,typename Data,typename Functor>
void postOrderScan(Node<Key,Data>* root,  const Functor& handle){
    if(root == nullptr) return;
    postOrderScan(root->left, handle);
    postOrderScan(root->right, handle);
    handle(root);
}





template<typename Key, typename Data>
Node<Key,Data>* AvlTree<Key,Data>::binaryRemove(Node<Key,Data>* deleted){
    assert( deleted!=nullptr );
    Node<Key,Data>** deletedSrc = &root;
    
    if(deleted->parent != nullptr){
        deletedSrc = (deleted->parent->left == deleted)? &deleted->parent->left : &deleted->parent->right;
    }

    //bool deletedIsLeft = (deleted->parent->left == deleted);
    if( (deleted->left==nullptr) && (deleted->right==nullptr)  ){ 
    //meaning deleted is a leaf
        removeLeaf(deleted, deletedSrc);
    }
    else if( (deleted->left && !(deleted->right)) || (!(deleted->left) && deleted->right)){
    // meaning deleted has only one son
        removeVertexWithOneSon(deleted, deletedSrc);
    }
    else { assert(deleted->left && deleted->right); 
    // meaning deleted has two sons
        Node<Key, Data>* successor = findSuccessor(deleted);
        swapAvlNodes(deleted, successor);
        deleted = successor;
        deletedSrc = (deleted->parent->left == deleted)? &deleted->parent->left : &deleted->parent->right;

        assert(deleted->left == nullptr);
        if (deleted->right==nullptr) removeLeaf(deleted, deletedSrc);
        else removeVertexWithOneSon(deleted, deletedSrc);
    }

    Node<Key,Data>* lastOnTrack = deleted->parent;
    delete deleted;

    return lastOnTrack;
}

template  <typename Key,typename Data>
void AvlTree<Key,Data>::assureHight(Node<Key,Data>* nodeOnTrack){
    if(nodeOnTrack == nullptr){
        assert(root == nullptr);
        return;
    }
    int leftHight = (nodeOnTrack->left)? nodeOnTrack->left->subTreeHight : NO_HIGHT;
    int rightHight = (nodeOnTrack->right)? nodeOnTrack->right->subTreeHight : NO_HIGHT;

    nodeOnTrack->subTreeHight = (leftHight > rightHight)? leftHight : rightHight;
    nodeOnTrack->subTreeHight++;

}

template  <typename Key,typename Data>
void AvlTree<Key,Data>::assureBalance(Node<Key,Data>* nodeOnTrack){
    Node<Key,Data>* currentNode;

    switch (balanceOf(nodeOnTrack))
    {
    case GO_RIGHT:
        currentNode = nodeOnTrack->right;
        if(balanceOf(currentNode) <= 0) rotateLeft(currentNode);
        else{ assert(balanceOf(currentNode) == 1);
            currentNode = currentNode->left;
            rotateRight(currentNode);
            rotateLeft(currentNode); 

        }
        break;

    case GO_LEFT:
        currentNode = nodeOnTrack->left;
        if(balanceOf(currentNode) >= 0) rotateRight(currentNode);
        else{ assert(balanceOf(currentNode) == -1);
            currentNode = currentNode->right;
            rotateLeft(currentNode);
            rotateRight(currentNode);
        }
        break;
    }

    assureHight(nodeOnTrack);
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
    Node<Key,Data>* lastOnSearch = nullptr;
    Node<Key,Data>* exists = findNode(key,lastOnSearch);
    
    if( exists ) return false;
    Node<Key,Data>* nodeOnTrack = binaryInsert(key,data,lastOnSearch);
    
    do{
        //assureHight(nodeOnTrack);
        assureBalance(nodeOnTrack);//  <--should also check suc
        nodeOnTrack = nodeOnTrack->parent;
    //}while ( nodeOnTrack->parent );
    }while ( nodeOnTrack );
        
    return true;   
}





//return values: True for "sucssess", False for  "not exists".
template<typename Key,typename Data>
bool AvlTree<Key,Data>::remove(const Key& key){
    Node<Key,Data>* lastOnSearch = nullptr;
    Node<Key,Data>* exists = findNode(key,lastOnSearch);

    if( !exists ) return false;
    Node<Key,Data>* nodeOnTrack = binaryRemove(exists);

    if (nodeOnTrack == nullptr) return true; // incase root was removed and was the only node

    do{ //assureHight(nodeOnTrack);
        assureBalance(nodeOnTrack);//  <--should also check succsess
        nodeOnTrack = nodeOnTrack->parent;
    //}while ( nodeOnTrack->parent );
    }while ( nodeOnTrack );

    return true;
 }

//template<typename Key,typename Data>

 template<typename Key,typename Data>
 void AvlTree<Key,Data>::clear(){
    struct deleteNodeFunc{
        void operator()(Node<Key,Data>* node) const{
            delete node;
        }   
    };

    postOrderScan(this->root, deleteNodeFunc());
    root = nullptr;
 }

 template<typename Key,typename Data>
 AvlTree<Key,Data>::~AvlTree(){
     clear();
 }


