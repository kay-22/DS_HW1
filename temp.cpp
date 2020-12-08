#include <cassert>
#include <iostream>

#include "AvlTree.h"
#include "List.h"

using list::List;
using std::cout;
using std::endl;

int main(){
    //AvlTree<int,int> a();
    List<int> list;

    list.clear();
    assert(list.getSize() == 0);
    list.remove(list.find(3));
    list.pushFront(3);
    list.pushFront(2);
    list.pushFront(1);
    assert(list.getSize() == 3);

    
    List<int> list2 = list;

    cout << "list1: " << endl;
    for (List<int>::iterator it = list.begin(); it != list.end(); ++it) {
        int x = *it;
        std::cout << x << std::endl;
    }

    cout << "removed 1 from list1" << endl;
    list.remove(list.find(1));
    assert(!list.contains(1));

    cout << "list2: " << endl;
    for (List<int>::iterator it = list2.begin(); it != list2.end(); ++it) {
        std::cout << *it << std::endl;
    }

    cout << "list2 = list1 " << endl;
    list2 = list;

    cout << "list2: " << endl;
    for (List<int>::iterator it = list2.begin(); it != list2.end(); ++it) {
        std::cout << *it << std::endl;
    }

    std::cout << "nice" << std::endl;


    return 0;
}