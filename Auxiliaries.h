#ifndef AUXILIARIES_H
#define AUXILIARIES_H

#include "List.h"
#include "AvlTree.h"
#include <memory>
#include <functional>

static const int NO_CLASS = 0;

struct Lecture;
struct Course;
struct Time;
struct IntPair;


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


// avl tree with representing number "time"
struct Time {
    int time;
    avlTree::AvlTree<IntPair,list::List<Time>::iterator> classes;
    int numOfClasses;

    explicit Time(int time,int numOfClasses) : time(time),numOfClasses(numOfClasses) {}
};





struct Course{
    
    avlTree::AvlTree<IntPair,list::List<Time>::iterator> classes;
    int numOfClasses;

    Course(int courseID, int numOfClasses)
            : classes(avlTree::AvlTree<IntPair,list::List<Time>::iterator>::semiFullTree(numOfClasses)),
              numOfClasses(numOfClasses){

        IntPair i(courseID,0);
        std::function<void(avlTree::Node<IntPair,list::List<Time>::iterator>*)>  assignNodes = 
            [&i](avlTree::Node<IntPair,list::List<Time>::iterator>* node) {
                node->key = i;
                node->data = list::List<Time>::iterator::iteratorNull();
                i.second++;
            };

    avlTree::AvlTree<IntPair,list::List<Time>::iterator>::inOrder(classes.getRoot(), assignNodes);
    assert(i.second == numOfClasses);
    }
    
};


#endif    //AUXILIARIES_H


