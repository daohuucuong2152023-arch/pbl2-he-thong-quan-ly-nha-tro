#ifndef QUEUE_H
#define QUEUE_H
using namespace std;

template <class T>
struct QueueNode {
    T data;
    QueueNode<T>* next;
    QueueNode(const T& d);
};

template <class T>
class Queue {
private:
    QueueNode<T>* frontPtr;
    QueueNode<T>* rearPtr;
    int count;

public:
    Queue();
    ~Queue();

    void enqueue(const T& data);

    bool dequeue(T& result);

    void dequeue();

    bool front(T& result) const;

    int size() const;
    bool isEmpty() const;
};

template <class T>
QueueNode<T>::QueueNode(const T& d) : data(d), next(nullptr) {}

template <class T>
Queue<T>::Queue() : frontPtr(nullptr), rearPtr(nullptr), count(0) {}

template <class T>
Queue<T>::~Queue() {
    while (!isEmpty()) dequeue();
}

template <class T>
void Queue<T>::enqueue(const T& data) {
    QueueNode<T>* newNode = new QueueNode<T>(data);
    if (rearPtr == nullptr) {
        frontPtr = rearPtr = newNode;
    }
    else {
        rearPtr -> next = newNode;
        rearPtr = newNode;
    }
    count++;
}

template <class T>
bool Queue<T>::dequeue(T& result) {
    if (isEmpty()) return false;
    QueueNode<T>* toDelete = frontPtr;
    result = toDelete -> data;
    frontPtr = frontPtr->next;
    if (frontPtr == nullptr) rearPtr = nullptr;
    delete toDelete;
    count--;
    return true;
}

template <class T>
void Queue<T>::dequeue() {
    if (isEmpty()) return;
    QueueNode<T>* toDelete = frontPtr;
    frontPtr = frontPtr->next;
    if (frontPtr == nullptr) rearPtr = nullptr;
    delete toDelete;
    count--;
}

template <class T>
bool Queue<T>::front(T& result) const {
    if (isEmpty()) return false;
    result = frontPtr -> data;
    return true;
}

template <class T>
int Queue<T>::size() const { return count; }

template <class T>
bool Queue<T>::isEmpty() const { return count == 0; }

#endif