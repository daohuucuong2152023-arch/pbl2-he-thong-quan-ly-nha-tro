#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
using namespace std;

template <class T>
struct DLLNode {
    T data;
    DLLNode<T>* prev;
    DLLNode<T>* next;

    DLLNode(const T& d);
};

template <class T>
class DoublyLinkedList {
private:
    DLLNode<T>* head;
    DLLNode<T>* tail;
    int count;

public:
    DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList<T>& other);
    DoublyLinkedList<T>& operator = (const DoublyLinkedList<T>& other);
    ~DoublyLinkedList();

    void insertBack(const T& data);

    void insertFront(const T& data);

    bool removeIf(bool (*matchFn)(const T&));

    bool findIf(bool (*matchFn)(const T&), T& result) const;

    bool updateIf(bool (*matchFn)(const T&), void (*updateFn)(T&));

    template <class Func>
    void traverseForward(Func visit) const;

    template <class Func>
    void traverseBackward(Func visit) const;

    int size() const;
    bool isEmpty() const;
};

template <class T>
DLLNode<T>::DLLNode(const T& d) : data(d), prev(nullptr), next(nullptr) {}

template <class T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

template <class T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& other) : head(nullptr), tail(nullptr), count(0) {
    DLLNode<T>* cur = other.head;
    while (cur != nullptr) {
        insertBack(cur -> data);
        cur = cur -> next;
    }
}

template <class T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator = (const DoublyLinkedList<T>& other) {
    if (this == &other) return *this;

    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        DLLNode<T>* toDelete = cur;
        cur = cur->next;
        delete toDelete;
    }
    head = tail = nullptr;
    count = 0;

    cur = other.head;
    while (cur != nullptr) {
        insertBack(cur->data);
        cur = cur->next;
    }
    return *this;
}

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        DLLNode<T>* toDelete = cur;
        cur = cur -> next;
        delete toDelete;
    }
}

template <class T>
void DoublyLinkedList<T>::insertBack(const T& data) {
    DLLNode<T>* newNode = new DLLNode<T>(data);
    if (tail == nullptr) {
        head = tail = newNode;
    }
    else {
        tail -> next = newNode;
        newNode -> prev = tail;
        tail = newNode;
    }
    count++;
}

template <class T>
void DoublyLinkedList<T>::insertFront(const T& data) {
    DLLNode<T>* newNode = new DLLNode<T>(data);
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        newNode -> next = head;
        head -> prev = newNode;
        head = newNode;
    }
    count++;
}

template <class T>
bool DoublyLinkedList<T>::removeIf(bool (*matchFn)(const T&)) {
    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        if (matchFn(cur->data)) {
            if (cur->prev) {
                cur->prev->next = cur -> next;
            }
            else {
                head = cur -> next;
            }
            if (cur -> next) {
                cur -> next -> prev = cur -> prev;
            }
            else {
                tail = cur -> prev;
            }
            delete cur;
            count--;
            return true;
        }
        cur = cur -> next;
    }
    return false;
}

template <class T>
bool DoublyLinkedList<T>::findIf(bool (*matchFn)(const T&), T& result) const {
    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        if (matchFn(cur -> data)) {
            result = cur -> data;
            return true;
        }
        cur = cur -> next;
    }
    return false;
}

template <class T>
bool DoublyLinkedList<T>::updateIf(bool (*matchFn)(const T&), void (*updateFn)(T&)) {
    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        if (matchFn(cur -> data)) {
            updateFn(cur -> data);
            return true;
        }
        cur = cur -> next;
    }
    return false;
}

template <class T>
template <class Func>
void DoublyLinkedList<T>::traverseForward(Func visit) const {
    DLLNode<T>* cur = head;
    while (cur != nullptr) {
        visit(cur->data);
        cur = cur -> next;
    }
}

template <class T>
template <class Func>
void DoublyLinkedList<T>::traverseBackward(Func visit) const {
    DLLNode<T>* cur = tail;
    while (cur != nullptr) {
        visit(cur->data);
        cur = cur -> prev;
    }
}

template <class T>
int DoublyLinkedList<T>::size() const { return count; }

template <class T>
bool DoublyLinkedList<T>::isEmpty() const { return count == 0; }

#endif