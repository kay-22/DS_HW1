#include <assert.h>
#include "Exceptions.h"

static const int GO_RIGHT = -2;
static const int GO_LEFT = 2;
static const int  NO_HIGHT = -1;


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
    Data*    find(const Key& key);
    bool     insert(const Key& key, const Data& data);
    bool     remove(const Key& key);
    
    
    // Non-interface part:
    ~AvlTree(); //   <---TODO
    private:
    Node<Key,Data>* root;

    //AvlTree(const AvlTree& other) = delete;

    /* returns the required node or null if failed. last <- last checked node */
    Node<Key,Data>* findNode(const Key& key, Node<Key,Data>* last);
    /* inserts to the AvlTree like BinaryTreeInsert. assuming "not-exists". returns a poiner to the added node. */
    Node<Key,Data>* binaryInsert(const Key& key, const Data& data, Node<Key,Data>* last );
    /* removes from the AvlTree like BinaryTreeRemove. assuming "exists". returns a pointr to the lowest on track. */
    Node<Key,Data>* binaryRemove(Node<Key,Data>* deleted);
    /* assures that the balanceFactor of the node is correct by applying "role-over" if neccessary. */
    void assureBalance(Node<Key,Data>* nodeOnTrack);
    /* performs a simple rotation to the left/right -
       where pivot is the either the unbalanced vertex or the son of the unbalanced */
    void rotateLeft(Node<Key,Data>* pivotSon);
    void rotateRight(Node<Key,Data>* pivotSon);

};

//declerations:

    template  <typename Key,typename Data>
    Node<Key,Data>* findSuccessor(Node<Key,Data>* deleted);

    template  <typename Key,typename Data>
    void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u);

    template  <typename Key,typename Data>
    void swapNodePointers(Node<Key,Data>* ptr1, Node<Key,Data>* ptr2);

    template  <typename Key,typename Data>
    void removeLeaf(Node<Key,Data>* deleted, bool deletedIsLeft);

    template  <typename Key,typename Data>
    void removeVertexWithOneSon(Node<Key,Data>* deleted, bool deletedIsLeft);

    template  <typename Key,typename Data>
    void rotateRight(Node<Key,Data>* pivot);

    template  <typename Key,typename Data,typename Functor>
    void postOrderScan(Node<Key,Data>* root, Functor handle);

    template  <typename Key,typename Data>
    int balanceOf(Node<Key,Data>* nodeOnTrack);

    // template<typename Key,typename Data>
    // struct deleteNodeFunc;
    

//end declerations

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

//assuming that deleted has 2 sons
template  <typename Key,typename Data>
inline Node<Key,Data>* findSuccessor(Node<Key,Data>* predecessor){
    assert(predecessor->right && predecessor->left);
    
    Node<Key,Data> currentNode = predecessor->right;
    while (currentNode->left) {
        currentNode = currentNode->left;
    }

    assert(currentNode->key > predecessor->key);
    return currentNode;
}

template  <typename Key,typename Data>
inline void swapNodePointers(Node<Key,Data>* ptr1, Node<Key,Data>* ptr2){  
    Node<Key,Data>* temp;

    temp = ptr1;
    ptr1 = ptr2;
    ptr2 = temp;
}

template  <typename Key,typename Data>
inline void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u){
    swapNodePointers(v->parent,u->parent);
    swapNodePointers(v->right,u->right);
    swapNodePointers(v->left,u->left);
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

    assert( (last->left == nullptr) && (last->right == nullptr));
    if( key < last->key ){
        last->left = newNode;
    }
    else{ assert( key > last->key );
        last->right = newNode;
    }

    return newNode;

}

template<typename Key, typename Data>
inline void removeLeaf(Node<Key,Data>* deleted, bool deletedIsLeft){

    if (deletedIsLeft){
        deleted->parent->left = nullptr;
    }
    else {
        deleted->parent->right = nullptr;
    }
}


template<typename Key, typename Data>
inline void removeVertexWithOneSon(Node<Key,Data>* deleted, bool deletedIsLeft){
    if(deletedIsLeft && deleted->left) {deleted->parent->left= deleted->left;}
    else if (deletedIsLeft && deleted->right) {deleted->parent->left= deleted->right;}
    else if (!deletedIsLeft && deleted->left) {deleted->parent->right= deleted->left;}
    else { assert(!deletedIsLeft && deleted->left);
        deleted->parent->right= deleted->right;
    }

}


template<typename Key, typename Data>
void AvlTree<Key,Data>::rotateRight(Node<Key,Data>* pivotSon){

    assert( pivotSon->parent );
    Node<Key,Data>* oldFather = pivotSon->parent;
    assert( oldFather->left==pivotSon );

    Node<Key,Data>* oldGrandFather = pivotSon->parent->parent;
    pivotSon->parent = oldGrandFather;
    if( oldGrandFather==nullptr ) { ////////////////////////////////parent is the root
        root = pivotSon;
    }else if( oldGrandFather->right==pivotSon->parent ){///////////////parent is a rightSon
        oldGrandFather->right = pivotSon;
    }else{ assert( oldGrandFather->left==pivotSon->parent ); //////////parent is a leftSon
        oldGrandFather->left = pivotSon;
    }

    oldFather->left = pivotSon->right;
    if( pivotSon->right!=nullptr ) pivotSon->right->parent = oldFather;
    pivotSon->right = oldFather;
    oldFather->parent = pivotSon;
}
    


template<typename Key, typename Data>
void AvlTree<Key,Data>::rotateLeft(Node<Key,Data>* pivotSon){

    assert( pivotSon->parent );
    Node<Key,Data>* oldFather = pivotSon->parent;
    assert( oldFather->right==pivotSon );

    Node<Key,Data>* oldGrandFather = pivotSon->parent->parent;
    pivotSon->parent = oldGrandFather;
    if( oldGrandFather==nullptr ) { ////////////////////////////////parent is the root
        root = pivotSon;
    }else if( oldGrandFather->right==pivotSon->parent ){///////////////parent is a rightSon
        oldGrandFather->right = pivotSon;
    }else{ assert( oldGrandFather->left==pivotSon->parent ); //////////parent is a leftSon
        oldGrandFather->left = pivotSon;
    }

    oldFather->right = pivotSon->left;
    if( pivotSon->left!=nullptr ) pivotSon->left->parent = oldFather;
    pivotSon->left = oldFather;
    oldFather->parent = pivotSon;
}
    



template  <typename Key,typename Data>
inline int balanceOf(Node<Key,Data>* nodeOnTrack){
    int leftHight = (nodeOnTrack->left)? nodeOnTrack->left->subTreeHight : NO_HIGHT;
    int rightHight = (nodeOnTrack->right)? nodeOnTrack->right->subTreeHight : NO_HIGHT;

    return leftHight - rightHight;
}





template  <typename Key,typename Data,typename Functor>
void postOrderScan(Node<Key,Data>* root, Functor handle){
    if(root == nullptr) return;
    postOrderScan(root->left);
    postOrderScan(root->right);
    handle(root);
}





template<typename Key, typename Data>
Node<Key,Data>* AvlTree<Key,Data>::binaryRemove(Node<Key,Data>* deleted){
    assert( deleted!=nullptr );

    bool deletedIsLeft = (deleted->parent->left == deleted);

    if( (deleted->left==nullptr) && (deleted->right==nullptr)  ){ 
    //meaning deleted is a leaf
        removeLeaf(deleted, deletedIsLeft);
    }
    else if( (deleted->left && !(deleted->right)) || (!(deleted->left) && deleted->right)){
    // meaning deleted has only one son
        removeVertexWithOneSon(deleted, deletedIsLeft);
    }
    else { assert(deleted->left && deleted->right); 
    // meaning deleted has two sons
        Node<Key, Data>* successor = findSuccessor(deleted);
        swapAvlNodes(deleted, successor);

        assert(deleted->left == nullptr);
        if (deleted->right==nullptr) removeLeaf(deleted);
        else removeVertexWithOneSon(deleted);
    }

    Node<Key,Data>* lastOnTrack = deleted->parent;
    delete deleted;

    return lastOnTrack;
}

template  <typename Key,typename Data>
void AvlTree<Key,Data>::assureBalance(Node<Key,Data>* nodeOnTrack){
    Node<Key,Data>* currentNode;

    switch (balanceOf(nodeOnTrack))
    {
    case GO_RIGHT:
        currentNode = nodeOnTrack->right;
        if(balanceOf(nodeOnTrack) <= 0) rotateLeft(currentNode);
        else{ assert(balanceOf(currentNode== 1));
            currentNode = currentNode->left;
            rotateLeft(currentNode);
            rotateRight(currentNode);
        }
        break;

    case GO_LEFT:
        currentNode = nodeOnTrack->left;
        if(balanceOf(nodeOnTrack) >= 0) rotateRight(currentNode);
        else{ assert(balanceOf(currentNode == -1));
            currentNode = currentNode->right;
            rotateRight(currentNode);
            rotateLeft(currentNode);
        }
        break;
    }
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
    Node<Key,Data>* nodeOnTrack = binaryInsert(key,data,lastOnSearch);
    
    do{ assureBalance(nodeOnTrack);//  <--should also check suc
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
    Node<Key,Data>* nodeOnTrack = binaryRemove(exists);

    do{ assureBalance(nodeOnTrack);//  <--should also check succsess
        nodeOnTrack = nodeOnTrack->parent;
    }while ( nodeOnTrack->parent );

    return true;
 }

//template<typename Key,typename Data>


 template<typename Key,typename Data>
 AvlTree<Key,Data>::~AvlTree(){

    struct deleteNodeFunc{
        void operator()(Node<Key,Data>* node){
            delete node;
        }   
    };

    postOrderScan(root, deleteNodeFunc());
 }

