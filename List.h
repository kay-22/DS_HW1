#ifndef LIST_H_
#define LIST_H_


namespace list{

    template <typename T>
    struct Node{
        T data;
        Node<T>* next;
        Node<T>* prev;

        explicit Node(const T& data, Node<T>* next = nullptr, Node<T>* prev = nullptr) 
            : data(data), next(next), prev(prev) {}
    };

    template <typename T>
    class List{

        Node<T>* head;
        Node<T>* tail;

        int size;


    public:
        class iterator;
        class const_iterator;

        List() : head(nullptr), tail(nullptr), size(0) {}
        List(const List&);
        ~List();
        List<T>& operator=(List);

        void clear();
        /* returns an iterator to the added node's data */
        iterator add(const T& data);
        /* returns an iterator to the node following the removed node. returns end() if the last node is removed */ 
        iterator remove(iterator);
        /* returns an iterator of the found node, or end() if didn't find */
        iterator find(const T& data);
        const_iterator find(const T& data) const;
        /* returns true if data is found, and false otherwise. */
        bool contains(const T& data) const;


        int getSize() const;
        bool isEmpty();


        /* returns an iterator to the head of the list */
        iterator begin();
        /* returns an iterator to the tail of the list */
        iterator end();
        /* returns an iterator to the head of the list */
        const_iterator begin() const;
        /* returns an iterator to the tail of the list */
        const_iterator end() const;


        /*  returns the previous iterator. tail will be returned if iterator's current node is nullptr.
            nullptr will be returned if iterator's current node is head */
        iterator getPrevious(iterator dataIterator);
        const_iterator getPrevious(const_iterator dataIterator) const;
    };




    template <typename T>
    class List<T>::iterator{
        const List* list;
        Node<T>* current; 
        friend class List;

        iterator(const List* list, Node<T>* current) : list(list), current(current) {}

    public:
        iterator(const iterator&) = default;
        iterator& operator=(const iterator&) = default;

        iterator& operator++() {
            if (current != nullptr) current = current->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator result = *this;
            ++*this;
            return result;
        }

        bool operator==(const iterator& other) const {
            assert(list == other.list);

            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        T& operator*() {
            assert(current != nullptr);
            return current->data;
        }
        
        const T& operator*() const{
            assert(current != nullptr);
            return current->data;
        }

        T* operator->() {
            return  &(operator*());
        }

        const T* operator->() const {
            return  &(operator*());
        }
    };



    template <typename T>
    class List<T>::const_iterator{
        const List* list;
        Node<T>* current; 
        friend class List;

        const_iterator(const List* list, Node<T>* current) : list(list), current(current) {}

    public:
        const_iterator(const const_iterator&) = default;
        const_iterator& operator=(const const_iterator&) = default;

        const_iterator& operator++() {
            if (current != nullptr) current = current->next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator result = *this;
            ++*this;
            return result;
        }

        bool operator==(const const_iterator& other) const {
            assert(list == other.list);

            return current == other.current;
        }
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
        
        const T& operator*() const{
            assert(current != nullptr);
            return current->data;
        }

        const T* operator->() const {
            return  &(operator*());
        }  
    };



    template <typename T>
    List<T>::List(const List<T>& other) : head(nullptr), tail(nullptr), size(0){

        for (const_iterator it = other.begin(); it != other.end(); it++) {
            add(*it);
        }

        assert(size == other.size);
    }




    template <typename T>
    List<T>::~List(){
            clear();
        }



    template <typename T>
    List<T>& List<T>::operator=(List other_copy){

        Node<T>* temp = head;
        head = other_copy.head;
        other_copy.head = temp;

        temp = tail;
        tail = other_copy.tail;
        other_copy.tail = tail;

        int temp_size = size;
        size = other_copy.size;
        other_copy.size = temp_size;

        return *this;
    }




    template <typename T>
    void List<T>::clear() {
        Node<T>* current = head;
        Node<T>* next = nullptr;
        int size = this->size;

        while (current != nullptr) {
            next = current->next;
            delete current;
            size--;
            current = next;
        }

        assert(size == 0);
    }





    template <typename T>
    typename List<T>::iterator List<T>::add(const T& data){
       // Node<T>* temp = head;
        //Node<T>* prev = nullptr;
        Node<T>* node = new Node<T>(data, head);

        if (head == nullptr) {
            assert(size == 0);
            tail = node;
        }

        node->next = head;
        head = node;

        if (head->next != nullptr) {
            head->next->prev = node;
        }
        
        size++;
        return begin();
    }





    template <typename T>
    typename List<T>::iterator List<T>::remove(iterator dataIterator) {
        if (dataIterator == end()) return end();

        iterator tempIt = dataIterator;
        if (++tempIt != end()){
            tempIt.current->prev = dataIterator.current->prev;
        }
        else {
            tail = dataIterator.current->prev;
        }

        tempIt = getPrevious(dataIterator);
        if ( tempIt.current != nullptr){
            tempIt.current->next = dataIterator.current->next;
        }
        else {
            head = dataIterator.current->next;
        }

        iterator result = getPrevious(dataIterator);
        delete dataIterator.current;

        size = (size != 0)? size - 1 : size;
        return result;
    }





    template <typename T>
    typename List<T>::iterator List<T>::find(const T& data){
        iterator it = begin();
        while (it != end()) {
            if (*it == data) break;
            ++it;
        }

        return it;
    }




    template <typename T>
    typename List<T>::const_iterator List<T>::find(const T& data) const{
        const_iterator it = begin();
        while (it != end()) {
            if (*it == data) break;
            ++it;
        }

        return it;
    }




    template <typename T>
    bool List<T>::contains(const T& data) const{
        return find(data) != end();
    }




    template <typename T>
    int List<T>::getSize() const{
        return size;
    }




    template <typename T>
    bool List<T>::isEmpty() {
        assert(size >= 0);
        return bool(size);
    }




    template <typename T>
    typename List<T>::iterator List<T>::begin() {
        return iterator(this, head);
    }
    


    
    template <typename T>
    typename List<T>::iterator List<T>::end() {
        return iterator(this, nullptr);
    }

    


    
    template <typename T>
    typename List<T>::const_iterator List<T>::begin() const{
        return const_iterator(this, head);
    }
    


    
    template <typename T>
    typename List<T>::const_iterator List<T>::end() const{
        return const_iterator(this, nullptr);
    }




    template <typename T>
    typename List<T>::iterator List<T>::getPrevious(iterator dataIterator) {
        if (dataIterator == end()) return iterator(this, tail);
        return iterator(this, dataIterator.current->prev);
    }




    template <typename T>
    typename List<T>::const_iterator List<T>::getPrevious(const_iterator dataIterator) const{
        if (dataIterator == end()) return const_iterator(this, tail);
        return const_iterator(this, dataIterator.current->prev);
    }



}// end of namespace list


#endif //LIST_H_