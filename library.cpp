#include"library.h"
#include"CoursesManager.h"
#include "Exceptions.h"

void* Init() 
{
    CoursesManager* DS;
    try {   
        DS = new CoursesManager ();
    }
    catch (...){
        DS = nullptr;
    }
     
    return (void*)DS;
}




StatusType AddCourse(void* DS, int courseID, int numOfClasses) 
{
    if (numOfClasses <= 0 || courseID <= 0 || DS == nullptr) {
        return INVALID_INPUT;
    }
    
    
    StatusType result = SUCCESS;
    try {
        if ((((CoursesManager*)DS)->AddCourse(courseID, numOfClasses)) == false) result = FAILURE;
    }
    catch(std::bad_alloc& e) {
        result = ALLOCATION_ERROR;
    }
    
    
    return result;
}



StatusType RemoveCourse(void* DS, int courseID){
    if(DS == nullptr || courseID <=0) return INVALID_INPUT;

    StatusType result = SUCCESS;
    if ((((CoursesManager*)DS)->RemoveCourse(courseID)) == false) result = FAILURE; 

    return result;
}

StatusType WatchClass(void *DS, int courseID, int classID, int time){
    if (time <= 0 || classID < 0 || courseID <= 0 || DS == nullptr) {
        return INVALID_INPUT;
    }
    
    StatusType result = SUCCESS;
    try {
        result = ((CoursesManager*)DS)->WatchClass(courseID, classID, time);
    }
    catch(std::bad_alloc& e) {
        result = ALLOCATION_ERROR;
    }

    return result;
}




StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed){
    if (classID < 0 || courseID <= 0 || DS == nullptr) {
        return INVALID_INPUT;
    }
    
    StatusType result = SUCCESS;
    try {
        result = ((CoursesManager*)DS)->TimeViewed(courseID, classID, timeViewed);
    }
    catch(std::bad_alloc& e) {
        result = ALLOCATION_ERROR;
    }

    return result;
}




StatusType GetMostViewedClasses(void *DS, int numOfClasses, int *courses, int *classes){
    if (numOfClasses <= 0 || DS == nullptr) {
        return INVALID_INPUT;
    }
    
    StatusType result = SUCCESS;
    try {
        result = ((CoursesManager*)DS)->GetMostViewedClasses(numOfClasses, courses, classes);
    }
    catch(std::bad_alloc& e) {
        result = ALLOCATION_ERROR;
    }

    return result;
}