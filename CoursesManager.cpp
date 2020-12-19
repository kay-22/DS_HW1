#include "CoursesManager.h"

using avlTree::AvlTree;
using list::List;
using std::function;




CoursesManager::CoursesManager() {
    timeAxis.pushBack(Time(0,NO_CLASS));
}





bool CoursesManager::AddCourse (int courseID, int numOfClasses){
    
    struct AddClasses {
        int i=0;
        List<Time>* timeAxis;
        AddClasses(List<Time>* timeAxis) : timeAxis(timeAxis) {}
        void operator()(avlTree::Node<int,List<Time>::iterator>* node){
            node->key = i++;
            node->data = timeAxis->begin();
        }
    };

    AddClasses addClasses(&timeAxis);
    Course newCourse(numOfClasses,timeAxis);
    if (courses.insert(courseID, newCourse) == false) return false;

    AvlTree<int,List<Time>::iterator> classes = AvlTree<int,List<Time>::iterator>::semiFullTree(numOfClasses);
    AvlTree<int,List<Time>::iterator>::inOrder(classes.getRoot(), addClasses);
    timeAxis.begin()->courses.insert(courseID,classes);
    timeAxis.begin()->numOfClasses += numOfClasses;
    return true;
}






bool CoursesManager::RemoveCourse(int courseID){
    Course* target = courses.find(courseID);

    if (target == nullptr) return false;

    // struct RemoveFromTimeAxis{
    //     List<Time>*const timeAxis;
    //     RemoveFromTimeAxis(List<Time>* timeAxis) : timeAxis(timeAxis) {}
    //     void operator()(avlTree::Node<IntPair,List<Time>::iterator>* node){
    //         assert(node->data != nullptr);
    //         List<Time>::iterator currentTime  = node->data;
    //         AvlTree<IntPair,List<Time>::iterator>& classes = currentTime->classes;
    //         classes.remove(node->key);
    //         --(currentTime->numOfClasses);
    //         if (classes.getRoot() == nullptr && currentTime->time != 0) {
    //             timeAxis->remove(currentTime);
    //         }
    //     }
    // };
    
    // RemoveFromTimeAxis removeFromTimeAxis(&timeAxis);
    // AvlTree<IntPair,List<Time>::iterator>::postOrder(target->classes.getRoot(), removeFromTimeAxis);
    //target->classes.clear(); make sure it doesn't leak

    for (int i = 0; i < target->numOfClasses; ++i) {
        List<Time>::iterator& currentTime = target->times[i];
        AvlTree<int,AvlTree<int,List<Time>::iterator>>& currentCoursesInTime = currentTime->courses;
        AvlTree<int,List<Time>::iterator>* courseToRemove = currentCoursesInTime.find(courseID);

        int numOfClassesInTime = (courseToRemove != nullptr)? courseToRemove->getSize() : 0;
        currentCoursesInTime.remove(courseID);
        currentTime->numOfClasses -= numOfClassesInTime;
        if (currentTime->numOfClasses == 0 && currentTime->time != 0) {
            timeAxis.remove(currentTime);
        }
    }
    courses.remove(courseID);

    return true;
}





StatusType CoursesManager::WatchClass(int courseID, int classID, int time){
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    assert(course->numOfClasses > classID);
    //course->times[classID] += time;
    //AvlTree<IntPair,List<Time>::iterator>& classes = course->classes;    
    //List<Lecture>::iterator& classPtr = *(classes.find(classID));
    List<Time>::iterator& classTime =  course->times[classID];
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
    //timeIterator->classes.insert(classKey, timeIterator);
    if (timeIterator->courses.find(courseID) == nullptr){
        timeIterator->courses.insert(courseID, AvlTree<int,List<Time>::iterator>());
    } 
    timeIterator->courses.find(courseID)->insert(classID, timeIterator);
    //++(timeIterator->numOfClasses);
    classTime->courses.find(courseID)->remove(classID);
    if (classTime->courses.find(courseID)->getSize() == 0){
        classTime->courses.remove(courseID);
    }
    if (classTime->courses.getSize() == 0 && classTime->time != 0) {
        timeAxis.remove(classTime);
    }
    --(classTime->numOfClasses);
    ++(timeIterator->numOfClasses);
    classTime = timeIterator;
    //if (course->maxTime->time < classTime->time) course->maxTime = classTime;

    return SUCCESS;
}




StatusType CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    *timeViewed  = course->times[classID]->time;

    return SUCCESS;
}




StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* coursesOutput, int* classesOutput){
    
    MostViewedOut out(coursesOutput, classesOutput);
    int stepsLeft = numOfClasses;

    function<void(avlTree::Node<int,AvlTree<int,List<Time>::iterator>>*)>  getCourse = 
    [&out, &numOfClasses, &stepsLeft](avlTree::Node<int,AvlTree<int,List<Time>::iterator>>* courseNode){
        function<void(avlTree::Node<int,List<Time>::iterator>*)>  getView = 
        [&out, &courseNode, &numOfClasses](avlTree::Node<int,List<Time>::iterator>* classNode){
            assert( out.i < numOfClasses );
            out.courses[out.i] = courseNode->key;
            out.classes[out.i++] = classNode->key;
        };

        if (courseNode->data.getSize() < stepsLeft) {
            avlTree::AvlTree<int,list::List<Time>::iterator>::inOrder(courseNode->data.getRoot(), getView);
            stepsLeft -= courseNode->data.getSize();
        }
        else{
            courseNode->data.stepByStepInOrder(stepsLeft, getView);
            stepsLeft = 0;
        }
    };

    List<Time>::iterator timeIterator = timeAxis.back();
    while (timeIterator != nullptr) { 
        timeIterator->courses.stepByStepInOrder(stepsLeft, getCourse);
        if( stepsLeft <= 0 ) break;
        timeIterator = timeAxis.getPrevious(timeIterator);
    }
    
    
    if( stepsLeft > 0 ) return FAILURE;
    return SUCCESS;
}