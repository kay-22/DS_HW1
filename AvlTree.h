#ifndef AVL_TREE_H_
#define AVL_TREE_H_


#include <assert.h>
#include <iostream>
#include <math.h>
#include "Exceptions.h"
#include "List.h"

static const int GO_RIGHT = -2;
static const int GO_LEFT = 2;
static const int  NO_HIGHT = -1;



namespace avlTree{

    template <typename Key, typename Data>
    struct Node;

   /************************* static functions declerations *************************/
    
        /**
        * finds a power of 2 that is closest to n.
        * @return
        * 	the requested 2Power
        * @throws
        * 	if n < 0
        */
        static inline int closest2Power(int n);

        /**
        * finds the node with the near'st key.
        * @return
        * a pointer to the successor node
        * @throws
        * 	if deleted has-not 2 sons
        */
        template  <typename Key,typename Data>
        static inline Node<Key,Data>* findSuccessor(Node<Key,Data>* deleted);

        /**
        * swaps the location in the tree for both v and u.
        */
        template  <typename Key,typename Data>
        static inline void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u);

        /**
        * a regular generic swap function
        */
        template  <typename T>
        static inline void swap(T& t1, T& t2);

        /**
        * prepare the tree for removing a node that is a leaf.
        */
        template  <typename Key,typename Data>
        static inline void removeLeaf(Node<Key,Data>* deleted, Node<Key,Data>** deltedSrc);

        /**
        * prepare the tree for removing a node that has one son.
        */
        template  <typename Key,typename Data>
        static inline void removeVertexWithOneSon(Node<Key,Data>* deleted, Node<Key,Data>** deltedSrc);

    
        /**
        * calculates:  left_subTree_hight - right_subTree_hight 
        *
        * @return
        * the blance-Factor of the nodeOnTrack
        */
        template  <typename Key,typename Data>
        static inline int balanceOf(Node<Key,Data>* nodeOnTrack);


    /********************* static functions declerations - END *************************/
    


   /**
    * Node - an auxiliary struct to represent a vertex in an avl tree.
    * left\right that is nullptr means subtree with hight=(-1)
    *----------------------------------------------------------------------------------------
    * Key          - a valvue of the index-place in a sorted container
    * Data         - The number of the classes that the Courses has in it
    * parent       - the source node in the graph
    * left         - the left sub-node in the graph
    * right        - the right sub-node in the graph
    * subTreeHight - the hight of the tree that ignores anyhing "above this-node" in the graph
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
        
        
        Node(const Node& other) = delete;
    };



    /**
    * AVL-Tree dictionary
    *
    * Implements a graph-container type. the container is sorted by some Key.
    * The type of the key and the value are generic.
    * The avl-tree is acssessible trough eighter its minimal node or its root.
    *
    * The following functions are available:
    *   AvlTree	- initializes a new empty avl-tree
    *   find   	- finds the data releted to the given Key
    *   insert 	- puts a given Data in the container in its place by the given Key
    *   remove 	- removes the data releted to the given Key
    */
    template  <typename Key,typename Data>
    class AvlTree{
    public:

        // Dictionary - interface part: -------------------------------------------------------
        //====================================================================================
        explicit AvlTree(Node<Key,Data>* root = nullptr);
        Data*    find(const Key& key);
        bool     insert(const Key& key, const Data& data);
        bool     remove(const Key& key);
        
        
        // Graph_type - interface part: ------------------------------------------------------
        //====================================================================================
        int getSize() {return size;}
        Node<Key,Data>*const getMinNode() { return minNode; }
        Node<Key,Data>*const getRoot() { return root; }

        //friend std::ostream& operator<<(std::ostream& os, const AvlTree<Key,Data>& tree);
        AvlTree(const AvlTree& other);
        AvlTree& operator=(AvlTree other);


        /* constructs returns a semi-full Tree in which all nodes are empty */
        static AvlTree semiFullTree(int nodesNum);
        
        template <typename Functor>
        static void postOrder(Node<Key,Data>* root, Functor& handle);
        template <typename Functor>
        static void preOrder(Node<Key,Data>* root,  Functor& handle);
        template <typename Functor>
        static void inOrder(Node<Key,Data>* root, Functor& handle);

        /* starts the inOrder tour in the Tree at the node with the lowest key */
        template <typename Functor>
        static void climbingInOrder(Node<Key,Data>* smallestLeaf, Functor& handle, int& steps);

        /* applies the inOrder tour in the Tree from the high'st key to the low'st key */
        template <typename Functor>
        static void reveresedPostOrder(Node<Key,Data>* root, Functor& handle);

        /* a controlled inOrder tour in the Tree with limited number of steps */
        template <typename Functor>
        bool static stepByStepInOrder(Node<Key,Data>* root, int& steps, Functor& handle);



        // NON-interface part: ---------------------------------------------------------
        //==============================================================================
        void clear();
        ~AvlTree();

        private:
        void copyAux(Node<Key,Data>* node, const Node<Key,Data>* const otherNode);
        Node<Key,Data>* root;
        Node<Key,Data>* minNode;
        int size;
        
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

        /*  performs a simple rotation to the left/right -
            where pivot is the either the unbalanced vertex or the son of the unbalanced */
        void rotateLeft(Node<Key,Data>* pivotSon);
        void rotateRight(Node<Key,Data>* pivotSon);

        /* expands a single node tree to full tree */
        static void expandToFullTree(Node<Key,Data>* node, int hight);

        /* updates the pointer of the minNode to the most left leaf*/
        void updateMinNode();

    };



    //by default: create an empty new AvlTree
    template  <typename Key,typename Data>
    AvlTree<Key,Data>::AvlTree(Node<Key,Data>* root) : root(root), minNode(root), size(0) {}



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




    static inline int closest2Power(int n){
        assert(n>=0);
        int twoPower = 1;
        int result = 0;

        while (n > twoPower-1){
            twoPower *= 2;
            result++;
        }

        return result;
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
        T temp = t1;
        t1 = t2;
        t2 = temp;
    }



    
    template  <typename Key,typename Data>
    static inline void swapAvlNodes(Node<Key,Data>* v, Node<Key,Data>* u){
        swap<Key>(v->key,u->key);    
        swap<Data>(v->data,u->data);    
    }


 
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




    template <typename Key,typename Data> 
    template <typename Functor>
    void AvlTree<Key,Data>::postOrder(Node<Key,Data>* root, Functor& handle){
        if(root == nullptr) return;
        postOrder(root->left, handle);
        postOrder(root->right, handle);
        handle(root);
    }




    template <typename Key,typename Data>
    template <typename Functor>
    void AvlTree<Key,Data>::inOrder(Node<Key,Data>* root, Functor& handle){
        if(root == nullptr) return;
        inOrder(root->left, handle);
        handle(root);
        inOrder(root->right, handle);
    }





    template <typename Key,typename Data>
    template <typename Functor>
    bool AvlTree<Key,Data>::stepByStepInOrder(Node<Key,Data>* root, int& steps, Functor& handle){
        if (steps < 0) return false;
        else if (steps == 0) return true;
        else if (root == nullptr) return true;

        list::List<Node<Key,Data>*> stack;
        Node<Key,Data>** current = &root;

        int counter = 0;
        //stack.pushFront(&root);

        assert(current != nullptr);
        do{
            if (*current != nullptr){
                stack.pushFront(*current);
                current = &((*current)->left);
            }
            else{
                typename list::List<Node<Key,Data>*>::iterator topIt = stack.begin();
                handle(*topIt);
                current = &((*topIt)->right);
                stack.remove(topIt);
                ++counter;  
            }
        }
        while (stack.getSize() !=0 && counter < steps);

        return true;
    }

    



    template <typename Key,typename Data>
    template <typename Functor>
    void AvlTree<Key,Data>::climbingInOrder(Node<Key,Data>* smallestLeaf, Functor& handle, int& steps){
        if (smallestLeaf == nullptr) return;

        Node<Key,Data>* current = smallestLeaf;
        while (current != nullptr && steps > 0) {
            handle(current);
            //--steps;
            stepByStepInOrder(current->right, steps, handle);
            current = current->parent;
        }
    }





    template <typename Key,typename Data>
    template <typename Functor>
    void AvlTree<Key,Data>::preOrder(Node<Key,Data>* root, Functor& handle){
        if(root == nullptr) return;
        handle(root);
        preOrder(root->left, handle);
        preOrder(root->right, handle);
    }




    template <typename Key,typename Data>
    template <typename Functor>
    void AvlTree<Key,Data>::reveresedPostOrder(Node<Key,Data>* root, Functor& handle){
        if(root == nullptr) return;
        reveresedPostOrder(root->right, handle);
        reveresedPostOrder(root->left, handle);
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
        
        ++size;
        updateMinNode();
        return true;   
    }




    //return values: True for "sucssess", False for  "not exists".
    template<typename Key,typename Data>
    bool AvlTree<Key,Data>::remove(const Key& key){
        Node<Key,Data>* lastOnSearch = nullptr;
        Node<Key,Data>* exists = findNode(key,lastOnSearch);

        if( !exists ) return false;
        Node<Key,Data>* nodeOnTrack = binaryRemove(exists);

        --size;

        if (nodeOnTrack == nullptr){    // incase root was removed and was the only node
            updateMinNode();
            return true; 
        } 

        do{ //assureHight(nodeOnTrack);
            assureBalance(nodeOnTrack);//  <--should also check succsess
            nodeOnTrack = nodeOnTrack->parent;
        //}while ( nodeOnTrack->parent );
        }while ( nodeOnTrack );

        updateMinNode();
        return true;
    }




    // template <typename Key,typename Data>
    // std::ostream& operator<<(std::ostream& os, const AvlTree<Key,Data>& tree){
    //         struct PrintData{
    //             void operator()(Node<Key,Data>* node) const{
    //                 std::cout << node->data << " " << balanceOf(node) << " " << node->subTreeHight << std::endl;
    //             }
    //         };
    //         PrintData printData;
    //         AvlTree<Key,Data>::inOrder(tree.root, printData);

    //         return os;
    // }




    template<typename Key,typename Data>
    AvlTree<Key,Data>::AvlTree(const AvlTree& other) : root(nullptr), minNode(nullptr), size(other.size) { 
        if (other.root == nullptr){
            return; //this root and minNode are already null
        }
        root = new Node<Key,Data>(other.root->key, other.root->data, 
                        nullptr, nullptr, nullptr, other.root->subTreeHight);
        copyAux(root, other.root);
    }




    template<typename Key,typename Data>
    AvlTree<Key,Data>& AvlTree<Key,Data>::operator=(AvlTree other){
        swap<Node<Key,Data>*>(this->root, other.root);
        swap<int>(size, other.size);
        return *this;
    }




    // constructs and returns a semi-full avl tree with
    // default homogeneous (Key,Data) for all nodes
    template <typename Key,typename Data>
    AvlTree<Key,Data> AvlTree<Key,Data>::semiFullTree(int nodesNum){    
        int hight = closest2Power(nodesNum) - 1;
        AvlTree<Key,Data> result;
        result.updateMinNode();
        if (hight == NO_HIGHT) return result;
        result.insert(Key(),Data());
        result.size = nodesNum;

        expandToFullTree(result.root, hight);

        struct RemoveExtraLeaves {
            AvlTree<Key,Data>* resTree;
            int currentExtra;
            RemoveExtraLeaves(AvlTree<Key,Data>* resTree, int currentExtra) 
                : resTree(resTree), currentExtra(currentExtra) {}
            void operator()(Node<Key,Data>* node){
                resTree->assureHight(node);
                if (currentExtra > 0 && node->subTreeHight == 0){
                    if (node->parent->left != nullptr){

                        if (node->parent->left == node) {           //node is a left son
                            node->parent->left = nullptr;
                        }
                        else { assert(node->parent->right == node); //node is a right son
                            node->parent->right = nullptr;
                        }
                    }
                    delete node;
                    currentExtra--;
                }
            }   
        };

        int extraLeaves = (pow(2,hight+1) - 1) - nodesNum;
        RemoveExtraLeaves removeExtraLeaves(&result, extraLeaves);
        reveresedPostOrder(result.root, removeExtraLeaves);

        result.updateMinNode();
        return result;
    }




    template <typename Key,typename Data>
    void AvlTree<Key,Data>::expandToFullTree(Node<Key,Data>* node, int hight){
        if (hight <= 0) return;
        node->left = new Node<Key,Data>(Key(), Data(), node, nullptr, nullptr, hight);
        node->right = new Node<Key,Data>(Key(), Data(), node, nullptr, nullptr, hight);

        expandToFullTree(node->left, hight-1);
        expandToFullTree(node->right, hight-1);
    }




    template <typename Key,typename Data>
    void AvlTree<Key,Data>::updateMinNode(){
        if (root == nullptr) {
            minNode = nullptr;
            return;
        }
        Node<Key,Data>* current = root;
        while (current->left != nullptr) current = current->left;
        minNode = current;
    }


    


    
    template<typename Key,typename Data>
    void AvlTree<Key,Data>::copyAux(Node<Key,Data>* node, const Node<Key,Data>* const otherNode){
        if (otherNode == nullptr) {
            assert(node == nullptr);
            return;
        }
        if (otherNode->left != nullptr) node->left = new Node<Key,Data>
                                        (otherNode->left->key, otherNode->left->data, node, 
                                        nullptr,nullptr, otherNode->left->subTreeHight);
        if (otherNode->right != nullptr) node->right = new Node<Key,Data>
                                        (otherNode->right->key, otherNode->right->data, node, 
                                        nullptr,nullptr, otherNode->right->subTreeHight);

        copyAux(node->left, otherNode->left);
        copyAux(node->right, otherNode->right);
    }



    template <typename Key,typename Data>
    void AvlTree<Key,Data>::clear(){
        
        struct DeleteNodeFunc{
            void operator()(Node<Key,Data>* node) const{
                delete node;
            }   
        };
        DeleteNodeFunc deleteNodeFunc;
        postOrder(this->root, deleteNodeFunc);
        root = nullptr;
    }




    template<typename Key,typename Data>
    AvlTree<Key,Data>::~AvlTree(){
        clear();
    }


} // end of namespace AvlTree


#endif //AVL_TREE_H_