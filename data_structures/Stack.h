#ifndef STACK_H
#define STACK_H

using namespace std;

template <class T>
struct StackNode {
    T data;
    StackNode<T>* next;
    StackNode(const T& d);
};

template <class T>
class Stack {
private:
    StackNode<T>* topPtr;
    int count;

public:
    Stack();
    ~Stack();

    void push(const T& data);
    bool pop(T& result);
    void pop();
    bool top(T& result) const;

    int size() const;
    bool isEmpty() const;
};

template <class T>
StackNode<T>::StackNode(const T& d) : data(d), next(nullptr) {}

template <class T>
Stack<T>::Stack() : topPtr(nullptr), count(0) {}

template <class T>
Stack<T>::~Stack() {
    while (!isEmpty()) pop();
}

template <class T>
void Stack<T>::push(const T& data) {
    StackNode<T>* newNode = new StackNode<T>(data);
    newNode->next = topPtr;
    topPtr = newNode;
    count++;
}

template <class T>
bool Stack<T>::pop(T& result) {
    if (isEmpty()) return false;
    StackNode<T>* toDelete = topPtr;
    result = toDelete->data;
    topPtr = topPtr->next;
    delete toDelete;
    count--;
    return true;
}

template <class T>
void Stack<T>::pop() {
    if (isEmpty()) return;
    StackNode<T>* toDelete = topPtr;
    topPtr = topPtr->next;
    delete toDelete;
    count--;
}

template <class T>
bool Stack<T>::top(T& result) const {
    if (isEmpty()) return false;
    result = topPtr->data;
    return true;
}

template <class T>
int Stack<T>::size() const { return count; }

template <class T>
bool Stack<T>::isEmpty() const { return count == 0; }

#endif