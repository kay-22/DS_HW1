#include <assert.h>

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
    explicit Node(Key key, Data data, Node* left = nullptr, Node* right= nullptr, subTreeHight = 0) 
        : key(key), data(data), left(left), right(right) subTreeHight(subTreeHight) {}    

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



/* 
    Node* root that is nulptr means an empty tree
 */
template  <typename T>
class AvlTree{
    Node<T>* root;

    AvlTree(Node<T>* root = nullptr) 
        : root(root) {}

    AvlTree(const AvlTree& other) = delete;

    // returns the required node or null if failed. last <- last checked node
    Node<T>* findNode(const T& key, Node<T>* last) {
        last = root;
        //...
        return nullptr;
    }

    

};