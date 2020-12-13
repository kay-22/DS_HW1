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

    if (target != nullptr) return false;

    struct RemoveFromTimeAxis{
        list::List<Time>*const timeAxis;
        RemoveFromTimeAxis(list::List<Time>* timeAxis) : timeAxis(timeAxis) {}
        void operator()(avlTree::Node<int,list::List<Lecture>::iterator>* node){
            assert(node->data != nullptr);
            list::List<Time>::iterator currentTime  = node->data->currentTime;
            list::List<Lecture> lectures = currentTime->lectures;
            lectures.remove(node->data);
            if (lectures.getSize() == 0) {
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