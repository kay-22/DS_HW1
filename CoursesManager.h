#ifndef COURSES_MANAGER_H_
#define COURSES_MANAGER_H_

#include "List.h"
#include "AvlTree.h"
#include "Auxiliaries.h"
#include "library.h"



/**
* CoursesManager - a data type to manage online-courses system.
* each course has its lectures, and each lecture has its time of view.
* in addition, all times of view are layed on a sorted time-axis - and acssesible from it.
*------------------------------------------------------------------------------------------
* courses  - an avl-tree of all the Courses in the system
* timeAxis - a two-way linked-list of all Times that represented on the systems time axis
*/
class CoursesManager {
    avlTree::AvlTree<int,Course> courses;
    list::List<Time> timeAxis;

public:

    // return values: True for "sucssess", False for  "already exists".
    // adds the course with all its lectures to the system with no watches at all
    bool AddCourse (int courseID, int numOfClasses);

    // return values: True for "sucssess", False for  "doesn't exists".
    // removes the course with all its lectures (with all their watches) from the system 
    bool RemoveCourse(int courseID);

    // returns the status of the action.
    // adds "time" to the time that the specific lecture has already got
    StatusType WatchClass(int courseID, int classID, int time);

    // returns the status of the action.
    // puts the time of view that related to courseId->classID inside *timeViewed
    StatusType TimeViewed(int courseID, int classID, int *timeViewed);

    // returns the status of the action.
    // puts the most viewed lectures in *classes and accordingly their courses in *courses
    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);

    CoursesManager();
    ~CoursesManager()=default;
};




#endif    //COURSES_MANAGER_H_


