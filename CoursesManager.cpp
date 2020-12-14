#include "CoursesManager.h"


CoursesManager::CoursesManager() {
    timeAxis.pushBack(Time(0));
}



bool CoursesManager::AddCourse (int courseID, int numOfClasses){
    
    struct AddLectures {
        int courseID;
        list::List<Time>* timeAxis;
        AddLectures(int courseID, list::List<Time>* timeAxis) : courseID(courseID), timeAxis(timeAxis) {}
        void operator()(avlTree::Node<int,list::List<Lecture>::iterator>* node){
            timeAxis->begin()->lectures.pushBack(Lecture(courseID, node->key, timeAxis->begin()));
            node->data = timeAxis->begin()->lectures.back();
        }
    };

    AddLectures addLectures(courseID, &timeAxis);
    Course newCourse(courseID, numOfClasses);
    avlTree::AvlTree<int,list::List<Lecture>::iterator>::inOrder(newCourse.lectures.getRoot(), addLectures);

    newCourse.maxTime = timeAxis.begin();
    courses.insert(courseID, newCourse);
    return true;
}

bool CoursesManager::RemoveCourse(int courseID){
    Course* target = courses.find(courseID);

    if (target == nullptr) return false;

    struct RemoveFromTimeAxis{
        list::List<Time>*const timeAxis;
        RemoveFromTimeAxis(list::List<Time>* timeAxis) : timeAxis(timeAxis) {}
        void operator()(avlTree::Node<int,list::List<Lecture>::iterator>* node){
            assert(node->data != nullptr);
            list::List<Time>::iterator currentTime  = node->data->currentTime;
            list::List<Lecture>& lectures = currentTime->lectures;
            lectures.remove(node->data);
            if (lectures.getSize() == 0 && currentTime->time != 0) {
                timeAxis->remove(currentTime);
            }
        }
    };
    
    RemoveFromTimeAxis removeFromTimeAxis(&timeAxis);
    avlTree::AvlTree<int,list::List<Lecture>::iterator>::postOrder(target->lectures.getRoot(), removeFromTimeAxis);
    //target->lectures.clear(); make sure it doesn't leak
    courses.remove(courseID);

    return true;
}





StatusType CoursesManager::WatchClass(int courseID, int classID, int time){
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    avlTree::AvlTree<int,list::List<Lecture>::iterator>& classes = course->lectures;    
    list::List<Lecture>::iterator& classPtr = *(classes.find(classID));
    list::List<Time>::iterator& classTime = classPtr->currentTime;
    list::List<Time>::iterator timeIterator = classTime;

    for (int i = 0; i < time; i++) {
        if (timeIterator == timeAxis.back()){
            timeIterator = timeAxis.pushBack(Time(classTime->time + time));
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
            timeIterator = timeAxis.pushAfter(timeIterator,Time(classTime->time + time));
            break;
        }
    }

    assert(timeIterator != timeAxis.end());
    timeIterator->lectures.pushBack(Lecture(courseID,classID,timeIterator));
    classTime->lectures.remove(classPtr);
    if (classTime->lectures.getSize() == 0 && classTime->time != 0) {
        timeAxis.remove(classTime);
    }
    classTime = timeIterator;
    if (course->maxTime->time < classTime->time) course->maxTime = classTime;

    return SUCCESS;
}




StatusType CoursesManager::TimeViewed(int courseID, int classID, int *timeViewed){
    Course* course = this->courses.find(courseID);
    if (course == nullptr) return FAILURE;
    if (classID +1 > course->numOfClasses) return INVALID_INPUT;

    list::List<Lecture>::iterator& classPtr = *(course->lectures.find(classID));
    *timeViewed = classPtr->currentTime->time;

    return SUCCESS;
}




StatusType CoursesManager::GetMostViewedClasses(int numOfClasses, int* coursesOutput, int* classesOutput){
    
    list::List<Time>::iterator timeIterator = timeAxis.back();
    while (timeIterator != nullptr) {
        for (Lecture lecture : timeIterator->lectures){
            
        }
        timeIterator = timeIterator->lectures.getPrevious(timeIterator);
    }
    
    return SUCCESS;
}