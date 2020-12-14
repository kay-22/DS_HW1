#ifndef AUXILIARIES_H
#define AUXILIARIES_H

#include "List.h"
#include "AvlTree.h"
#include <memory>
#include <functional>

//static const int NO_COURSE = -1;

struct Lecture;
struct Course;
struct Time;

// linked list, such that every element in it is a linked lists of lectures with time_views = this->time
struct Time {
    int time;
    list::List<Lecture> lectures;

    explicit Time(int time) : time(time) {}
};





//Lecture is a list element that holds an iterator of the current time_views
struct Lecture {
    int courseID; //for GetMostViewedClasses
    int classID;
    list::List<Time>::iterator currentTime;

    //Lecture() 
    //    : currentTime(list::List<Time>::const_iterator::iteratorNull()), courseID(NO_COURSE) {}

    Lecture(int courseID, int classID, const list::List<Time>::iterator& currentTime)
        : courseID(courseID), classID(classID), currentTime(currentTime) {}
};






struct Course{
    avlTree::AvlTree<int,list::List<Lecture>::iterator> lectures;
    list::List<Time>::iterator maxTime;
    int numOfClasses;

    Course(int courseID, int numOfClasses)
            : lectures(avlTree::AvlTree<int,list::List<Lecture>::iterator>::semiFullTree(numOfClasses)),
              maxTime( list::List<Time>::iterator::iteratorNull() ),
              numOfClasses(numOfClasses){

        int i = 0;
        std::function<void(avlTree::Node<int,list::List<Lecture>::iterator>*)>  assignNodes = 
            [&i](avlTree::Node<int,list::List<Lecture>::iterator>* node) {
                node->key = i;
                node->data = list::List<Lecture>::iterator::iteratorNull();
                i++;
            };

    avlTree::AvlTree<int, list::List<Lecture>::iterator>::inOrder(lectures.getRoot(), assignNodes);
    assert(i == numOfClasses);
    }
    
};


#endif    //AUXILIARIES_H


