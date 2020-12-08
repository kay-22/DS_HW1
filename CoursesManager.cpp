#include "CoursesManager.h"


CoursesManager::CoursesManager() {
    timeAxis.pushBack(Time(0));
}



bool CoursesManager::AddCourse (int courseID, int numOfClasses){
    Course newCourse(courseID, numOfClasses);
    courses.insert(courseID, newCourse);

    struct AddLectures {
        int courseID;
        list::List<Time>* timeAxis;
        AddLectures(int courseID, list::List<Time>* timeAxis) : courseID(courseID), timeAxis(timeAxis) {}
        void operator()(avlTree::Node<int,list::List<Lecture>::iterator>* node){
            timeAxis->begin()->lectures.pushFront(Lecture(courseID, *timeAxis));
            node->data = timeAxis->begin()->lectures.begin();
        }
    };

    AddLectures addLectures(courseID, &timeAxis);
    avlTree::AvlTree<int,list::List<Lecture>::iterator>::inOrder(newCourse.lectures.getRoot(), addLectures);

    newCourse.maxTime = timeAxis.begin();
    return true;
}
