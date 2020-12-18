#ifndef AUXILIARIES_H
#define AUXILIARIES_H

#include "List.h"
#include "AvlTree.h"
#include <memory>
#include <functional>

static const int NO_CLASS = 0;

struct Course;
struct Time;
struct GetView;
struct GetCourse;
struct MostViewedOut;


struct IntPair{
    int first = 0;
    int second = 0;
    explicit IntPair(int first=0, int second=0) : first(first), second(second) {}
    bool operator<(const IntPair& other) const{
        if (first < other.first){
            return true;
        }
        else if (first == other.first){
            if (second < other.second){
                return true;
            }
        }
        return false;
    }
    bool operator==(const IntPair& other) const {
        return ((first == other.first) && (second == other.second));
    }
    bool operator>(const IntPair& other)const {
        return (!(*this==other) && !(*this < other));
    }
    bool operator!=(const IntPair& other) const {
        return !(*this == other);
    }
    bool operator>=(const IntPair& other) const {
        return !(*this < other);
    }
    bool operator<=(const IntPair& other) const {
        return !(*this > other);
    }

};


// // avl tree with representing number "time"
// struct Time {
//     int time;
//     avlTree::AvlTree<IntPair,list::List<Time>::iterator> classes;
//     int numOfClasses;

//     explicit Time(int time,int numOfClasses) : time(time),numOfClasses(numOfClasses) {}
// };


struct Time {
    int time;
    int numOfClasses;
    avlTree::AvlTree<int, avlTree::AvlTree<int,list::List<Time>::iterator> > courses;

    explicit Time(int time,int numOfClasses) : time(time), numOfClasses(numOfClasses) {}
};






struct Course{
    list::List<Time>::iterator* times;
    int numOfClasses;

    explicit Course(int numOfClasses, list::List<Time>& timeAxis) 
        : times(new list::List<Time>::iterator[numOfClasses]), numOfClasses(numOfClasses){  
        for (int i = 0; i<numOfClasses; i++){
            times[i] = timeAxis.begin();
        }
    }

    Course(const Course& other) 
        : times(new list::List<Time>::iterator[other.numOfClasses]), numOfClasses(other.numOfClasses){
        for (int i = 0; i<numOfClasses; i++) {
            times[i] = other.times[i];
        }
    }

    Course& operator=(Course other){
        avlTree::swap<list::List<Time>::iterator*>(times, other.times);
        
        return *this;
    }

    ~Course(){
        delete[] times;
    }
};










// struct IntArray{
//     int *array;
//     int size;

//     explicit IntArray(int size) : array(new int[size]), size(size){  
//         for (int i = 0; i<size; i++) array[i] = 0;
//     }

//     IntArray(const IntArray& other) : array(new int[other.size]){
//         for (int i = 0; i<size; i++) array[i] = other.array[i];
//     }

//     IntArray& operator=(IntArray other){
//         int* temp = array;
//         array = other.array;
//         size = other.size;
//         other.array = temp;
//         return *this;
//     }

//     ~IntArray(){
//         delete array;
//     }
// };


// struct Course{
    
//     avlTree::AvlTree<IntPair,list::List<Time>::iterator> classes;
//     int numOfClasses;
//     IntArray classesTimes;

//     Course(int courseID, int numOfClasses)
//             : classes(avlTree::AvlTree<IntPair,list::List<Time>::iterator>::semiFullTree(numOfClasses)),
//               numOfClasses(numOfClasses), classesTimes(numOfClasses){

//         IntPair i(courseID,0);
//         std::function<void(avlTree::Node<IntPair,list::List<Time>::iterator>*)>  assignNodes = 
//             [&i](avlTree::Node<IntPair,list::List<Time>::iterator>* node) {
//                 node->key = i;
//                 node->data = list::List<Time>::iterator::iteratorNull();
//                 i.second++;
//             };

//     avlTree::AvlTree<IntPair,list::List<Time>::iterator>::inOrder(classes.getRoot(), assignNodes);
//     assert(i.second == numOfClasses);
//     }
    
// };


// struct GetView{
//     const int numOfClasses;
//     const int courseID;
//     MostViewedOut out;

//     GetView(int numOfClasses, int courseID, int* coursesOutput,int* classesOutput, int* i):
//             numOfClasses(numOfClasses), courseID(courseID), coursesOutput(coursesOutput),classesOutput(classesOutput), i(i){}
//     void operator()(avlTree::Node<int,list::List<Time>::iterator>* node){
//         assert( i < numOfClasses );
//         coursesOutput[i] = courseID;
//         classesOutput[i++] = node->key;
//         assert( i < numOfClasses );
//     }
// };





// struct GetCourse{
//     int* steps; 
//     int numOfClasses;
//     MostViewedOut out;

//     GetCourse(int* steps, int numOfClasses, MostViewedOut out)
//         : steps(steps), numOfClasses(numOfClasses), out(out){}
//     void operator()(avlTree::Node<int,avlTree::AvlTree<int,list::List<Time>::iterator>>* node){
//         GetView getView(numOfClasses, node->key, out);
//         if (node->data.getSize() < *steps) {
//             avlTree::AvlTree<int,list::List<Time>::iterator>::inOrder(node->data.getRoot(), getView);
//             *steps -= node->data.getSize();
//         }
//         else{
//             node->data.stepByStepInOrder(*steps, getView);
//             *steps = 0;
//         }
//     }

// };

struct MostViewedOut{
    int *const courses;
    int *const classes;
    int i = 0;

    MostViewedOut(int* courses, int* classes) : courses(courses), classes(classes) {}
};


#endif    //AUXILIARIES_H


