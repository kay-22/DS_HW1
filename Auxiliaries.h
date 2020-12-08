#ifndef AUXILIARIES_H
#define AUXILIARIES_H

#include "List.h"
#include "AvlTree.h"
#include <memory>

// linked list, such that every element in it is a linked lists of lectures with time_views = this->time
struct Time {
    int time;
    list::List<Lecture> lectures;
};

//Lecture is a list element that holds an iterator of the current time_views
struct Lecture {
    list::List<Time>::const_iterator currentTime;
    int courseID;

    Lecture(const list::List<Time>& timeAxis, int courseID) 
        : currentTime(timeAxis.begin()), courseID(courseID) {}
};

struct Course {
    avlTree::AvlTree<int,Lecture*> lectures;
    list::List<Time>::iterator maxTime;

    Course(int numOfClasses) /* : lectures(AvlTree<int,Lecture*>::semiFullTree(numOfClasses) */ {
    //todo: create array from zero to numOfClasses 
    //      make sure time axis is updated
    //      create a corresponding array of data
    //------------------------------------------------------------------------------------------------------
    }
};


#endif    //AUXILIARIES_H


