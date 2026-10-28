#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
using namespace std;


template <typename T>
struct Node {
    T data;          
    Node<T>* prev;      
    Node<T>* next;     

    // Constructor của Node
    Node(T data) {
        this->data = data;
        this->prev = nullptr;
        this->next = nullptr;
    }
};

template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;     
    Node<T>* tail;   
    int size;          

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    DoublyLinkedList(const DoublyLinkedList<T>& other)
        : head(nullptr), tail(nullptr), size(0) {
        for (Node<T>* cur = other.head; cur != nullptr; cur = cur->next) {
            addBack(cur->data);
        }
    }

    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other) {
        if (this == &other) return *this;
        DoublyLinkedList<T> copy(other);
        Node<T>* oldHead = head;
        Node<T>* oldTail = tail;
        int oldSize = size;
        head = copy.head;
        tail = copy.tail;
        size = copy.size;
        copy.head = oldHead;
        copy.tail = oldTail;
        copy.size = oldSize;
        return *this;
    }

    void insertFront(const T& data) { addFront(data); }
    void insertBack(const T& data) { addBack(data); }

    template <typename Predicate>
    bool findIf(Predicate predicate, T& result) const {
        for (Node<T>* cur = head; cur != nullptr; cur = cur->next) {
            if (predicate(cur->data)) {
                result = cur->data;
                return true;
            }
        }
        return false;
    }

    template <typename Predicate, typename Updater>
    bool updateIf(Predicate predicate, Updater updater) {
        for (Node<T>* cur = head; cur != nullptr; cur = cur->next) {
            if (predicate(cur->data)) {
                updater(cur->data);
                return true;
            }
        }
        return false;
    }

    template <typename Predicate>
    bool removeIf(Predicate predicate) {
        int index = 0;
        for (Node<T>* cur = head; cur != nullptr; cur = cur->next, ++index) {
            if (predicate(cur->data)) return removeAt(index);
        }
        return false;
    }

    template <typename Visitor>
    void traverseForward(Visitor visitor) const {
        for (Node<T>* cur = head; cur != nullptr; cur = cur->next) {
            visitor(static_cast<const T&>(cur->data));
        }
    }

    template <typename Visitor>
    void traverseBackward(Visitor visitor) const {
        for (Node<T>* cur = tail; cur != nullptr; cur = cur->prev) {
            visitor(static_cast<const T&>(cur->data));
        }
    }


    void addFront(T data);     
    void addBack(T data);      

  
    bool removeAt(int index);  
    Node<T>* getHead() const;
    Node<T>* getTail() const;
    Node<T>* getNodeAt(int index) const;   
    int getSize() const;
    bool isEmpty() const;


    void displayAll() const;   
};


template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}


template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* nextNode = current->next; 
        delete current;                      
        current = nextNode;                
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}


template <typename T>
void DoublyLinkedList<T>::addFront(T data) {
    Node<T>* newNode = new Node<T>(data);

    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;   
        head->prev = newNode; 
        head = newNode;         
    }
    size++;
}

template <typename T>
void DoublyLinkedList<T>::addBack(T data) {
    Node<T>* newNode = new Node<T>(data);

    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;   
        tail->next = newNode;   
        tail = newNode;      
    }
    size++;
}
template <typename T>
bool DoublyLinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= size) {
        return false;
    }

    Node<T>* toDelete = getNodeAt(index);
    if (toDelete == nullptr) return false;

    if (size == 1) {
        head = nullptr;
        tail = nullptr;
    }

    else if (toDelete == head) {
        head = head->next;      
        head->prev = nullptr; 
    }

    else if (toDelete == tail) {
        tail = tail->prev;      
        tail->next = nullptr;  
    }
  
    else {
        toDelete->prev->next = toDelete->next;  
        toDelete->next->prev = toDelete->prev; 
    }

    delete toDelete;    
    size--;
    return true;
}


template <typename T>
Node<T>* DoublyLinkedList<T>::getHead() const {
    return head;
}


template <typename T>
Node<T>* DoublyLinkedList<T>::getTail() const {
    return tail;
}


template <typename T>
Node<T>* DoublyLinkedList<T>::getNodeAt(int index) const {
    if (index < 0 || index >= size) {
        return nullptr;
    }

    Node<T>* current;

  
    if (index <= size / 2) {

        current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = tail;
        for (int i = size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    return current;
}


template <typename T>
int DoublyLinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool DoublyLinkedList<T>::isEmpty() const {
    return size == 0;
}


template <typename T>
void DoublyLinkedList<T>::displayAll() const {
    if (isEmpty()) {
        cout << "(Danh sach rong)" << endl;
        return;
    }

    Node<T>* current = head;
    int index = 0;
    while (current != nullptr) {
        cout << "[" << index << "] ";
        current->data.display(); 
        cout << endl;
        current = current->next;
        index++;
    }
}

#endif 
