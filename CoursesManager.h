#ifndef COURSES_MANAGER_H_
#define COURSES_MANAGER_H_

#include "List.h"
#include "AvlTree.h"
#include "Auxiliaries.h"

class CoursesManager {
    avlTree::AvlTree<int,Course> courses;
    list::List<Time> timeAxis;

    
};

#endif    //COURSES_MANAGER_H_


