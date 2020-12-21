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



/**
* Time - an auxiliary struct for describing
* a specific time in which an event occured.
*--------------------------------------------------------------------------
* time         - a number in the time-line.
* numOfClasses - The number of the classes that the Time has in it.
* courses      - a courses-tree of lectures-trees, ONLY the ones releted to Time.time .
*/
struct Time {
    int time;
    int numOfClasses;
    avlTree::AvlTree<int, avlTree::AvlTree<int,list::List<Time>::iterator> > courses;

    explicit Time(int time,int numOfClasses) : time(time), numOfClasses(numOfClasses) {}
};





/**
* Course - an auxiliary struct for describing
* a specific course with All its lectures.
*--------------------------------------------------------------------------
* times        - a lectures-array where each cell is an iterator to some Time
* numOfClasses - The number of the classes that the Courses has in it
*/
struct Course{
    list::List<Time>::iterator* times;
    int numOfClasses;

    explicit Course(int numOfClasses, list::List<Time>& timeAxis)  
        : times(new list::List<Time>::iterator[numOfClasses]), numOfClasses(numOfClasses){  
        for (int i = 0; i<numOfClasses; i++) times[i] = timeAxis.begin();
    }

    Course(const Course& other) 
        : times(new list::List<Time>::iterator[other.numOfClasses]), numOfClasses(other.numOfClasses){
        for (int i = 0; i<numOfClasses; i++) {
            times[i] = other.times[i];
        }
    }

    Course& operator=(Course other){
        avlTree::swap<list::List<Time>::iterator*>(times, other.times); 
        avlTree::swap<int>(numOfClasses, other.numOfClasses);   
        return *this;
    }

    ~Course(){
        delete[] times;
    }
};




/**
* MostViewedOut - an auxiliary struct for preparing
* the output of GetMostViewedClasses(DS,numOfClasses,courses,classes)
*--------------------------------------------------------------------------
* i - helps indexing the the arrays
*/
struct MostViewedOut{
    int *const courses;
    int *const classes;
    int i = 0;

    MostViewedOut(int* courses, int* classes) : courses(courses), classes(classes) {}
};


#endif    //AUXILIARIES_H


