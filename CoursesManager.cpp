#include "CoursesManager.h"

using avlTree::AvlTree;
using list::List;




CoursesManager::CoursesManager() {
    timeAxis.pushBack(Time(0,NO_CLASS));
}





bool CoursesManager::AddCourse (int courseID, int numOfClasses){
    
    struct AddClasses {
        int courseID;
        List<Time>* timeAxis;
        AddClasses(int courseID, List<Time>* timeAxis) : courseID(courseID), timeAxis(timeAxis) {}
        void operator()(avlTree::Node<IntPair,List<Time>::iterator>* node){
            node->data = timeAxis->begin();
            timeAxis->begin()->classes.insert(node->key,node->data);
            ++(timeAxis->begin()->numOfClasses);
        }
    };

    AddClasses addClasses(courseID, &timeAxis);
    Course newCourse(courseID, numOfClasses);
    AvlTree<IntPair,List<Time>::iterator>::inOrder(newCourse.classes.getRoot(), addClasses);

    //newCourse.maxTime = timeAxis.begin();
    courses.insert(courseID, newCourse);
    return true;
}






bool CoursesManager::RemoveCourse(int courseID){
    Course* target = courses.find(courseID);

    if (target == nullptr) return false;

    struct RemoveFromTimeAxis{
        List<Time>*const timeAxis;
        RemoveFromTimeAxis(List<Time>* timeAxis) : timeAxis(timeAxis) {}
        void operator()(avlTree::Node<IntPair,List<Time>::iterator>* node){
            assert(node->data != nullptr);
            List<Time>::iterator currentTime  = node->data;
            AvlTree<IntPair,List<Time>::iterator>& classes = currentTime->classes;
            classes.remove(node->key);
            --(currentTime->numOfClasses);
            if (classes.getRoot() == nullptr && currentTime->time != 0) {
                timeAxis->remove(currentTime);
            }
        }
    };
    
    RemoveFromTimeAxis removeFromTimeAxis(&timeAxis);
    AvlTree<IntPair,List<Time>::iterator>::postOrder(target->classes.getRoot(), removeFromTimeAxis);
    //target->classes.clear(); make sure it doesn't leak
    courses.remove(courseID);

    return true;
}





StatusType CoursesManager::WatchClass(int courseID, int classID, int time){
    IntPair classKey(courseID,classID);
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    AvlTree<IntPair,List<Time>::iterator>& classes = course->classes;    
    //List<Lecture>::iterator& classPtr = *(classes.find(classID));
    List<Time>::iterator& classTime =  *(classes.find(classKey));
    List<Time>::iterator timeIterator = classTime;

    for (int i = 0; i < time; i++) {
        if (timeIterator == timeAxis.back()){
            timeIterator = timeAxis.pushBack(Time(classTime->time + time,NO_CLASS));
            break;
        }
        if (timeIterator->time < classTime->time + time){
            ++timeIterator;
        }
        else if (timeIterator->time == classTime->time + time){
            break;
        }
        else{

            timeIterator = timeAxis.getPrevious(timeIterator);
            timeIterator = timeAxis.pushAfter(timeIterator,Time(classTime->time + time,NO_CLASS));
            break;
        }
    }

    assert(timeIterator != timeAxis.end());
    timeIterator->classes.insert(classKey, timeIterator);
    ++(timeIterator->numOfClasses);
    classTime->classes.remove(classKey);
    --(classTime->numOfClasses);
    if (classTime->classes.getRoot() == nullptr && classTime->time != 0) {
        timeAxis.remove(classTime);
    }
    classTime = timeIterator;
    //if (course->maxTime->time < classTime->time) course->maxTime = classTime;

    return SUCCESS;
}




StatusType CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    *timeViewed  = (*(course->classes.find(IntPair(courseID, classID))))->time;

    return SUCCESS;
}




StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* coursesOutput, int* classesOutput){

    struct GetView{
        const int numOfClasses;
        int *const coursesOutput;
        int *const classesOutput;
        int i = 0;
        GetView(int numOfClasses,int* coursesOutput,int* classesOutput):
                numOfClasses(numOfClasses),coursesOutput(coursesOutput),classesOutput(classesOutput){}
        void operator()(avlTree::Node<IntPair,List<Time>::iterator>* node){
            if( i < numOfClasses ){
                coursesOutput[i] = node->key.first;
                classesOutput[i++] = node->key.second;
                assert( i < numOfClasses );
            }
        }
    };
    GetView getView(numOfClasses,coursesOutput,classesOutput);
    

    List<Time>::iterator timeIterator = timeAxis.back();
    while (timeIterator != nullptr) { 
        AvlTree<IntPair,List<Time>::iterator>::inOrder(timeIterator->classes.getRoot(),getView);
        if( getView.i == numOfClasses-1 ) break;
        timeIterator = timeAxis.getPrevious(timeIterator);
    }
    
    
    if( getView.i < numOfClasses-1 ) return FAILURE;
    return SUCCESS;
}