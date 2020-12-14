#include <cassert>
#include <iostream>
#include <functional>

#include "AvlTree.h"
#include "List.h"
#include "CoursesManager.h"
#include "library.h"

#define NUMBER_OF_TESTS 1

using list::List;
using std::cout;
using std::endl;

bool testAddAndRemove() {   //test 1
    CoursesManager* cmTest = (CoursesManager*)Init();
    
    for (int i = 1; i < 6; i++) {
        if (AddCourse(cmTest, i, 3) != SUCCESS) {
        return false;
        }
    }

    if (RemoveCourse(cmTest, 3) != SUCCESS) {
        return false;
    }

    if (AddCourse(cmTest, 3, 5) != SUCCESS) {
        return false;
    }

    if (AddCourse(cmTest, 3, 0) != INVALID_INPUT) {
        return false;
    }

    return true;
}

void run_test(std::function<bool()> test, std::string test_name){
    if(!test()){
        std::cout<<test_name<<" FAILED."<<std::endl;
        return;
    }
    std::cout<<test_name<<" SUCCEEDED."<<std::endl;

}

int main(){
    std::function<bool()> tests[NUMBER_OF_TESTS]= {testAddAndRemove};
    for(int i=0;i<NUMBER_OF_TESTS;++i){
        run_test(tests[i],"Test "+std::to_string(i+1));
    }
}
// int main(){
//     //AvlTree<int,int> a();
//     List<int> list;

//     list.clear();
//     assert(list.getSize() == 0);
//     list.remove(list.find(3));
//     list.pushFront(3);
//     list.pushFront(2);
//     list.pushFront(1);
//     assert(list.getSize() == 3);

    
//     List<int> list2 = list;

//     cout << "list1: " << endl;
//     for (List<int>::iterator it = list.begin(); it != list.end(); ++it) {
//         int x = *it;
//         std::cout << x << std::endl;
//     }

//     cout << "removed 1 from list1" << endl;
//     list.remove(list.find(1));
//     assert(!list.contains(1));

//     cout << "list2: " << endl;
//     for (List<int>::iterator it = list2.begin(); it != list2.end(); ++it) {
//         std::cout << *it << std::endl;
//     }

//     cout << "list2 = list1 " << endl;
//     list2 = list;

//     cout << "list2: " << endl;
//     for (List<int>::iterator it = list2.begin(); it != list2.end(); ++it) {
//         std::cout << *it << std::endl;
//     }

//     std::cout << "nice" << std::endl;


//     return 0;
// }