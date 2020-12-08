#ifndef COURSES_MANAGER_H_
#define COURSES_MANAGER_H_

#include "List.h"
#include "AvlTree.h"
#include "Auxiliaries.h"
#include "library.h"



class CoursesManager {
    avlTree::AvlTree<int,Course> courses;
    list::List<Time> timeAxis;

public:
    CoursesManager();

    //return values: True for "sucssess", False for  "already exists".
    bool AddCourse (int courseID, int numOfClasses);

    StatusType RemoveCourse(int courseID);

    StatusType WatchClass(int courseID, int classID, int time);

    StatusType TimeViewed(int courseID, int classID, int *timeViewed);

    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);

    ~CoursesManager();
};




#endif    //COURSES_MANAGER_H_


