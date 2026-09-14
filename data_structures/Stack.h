#ifndef STACK_H
#define STACK_H

using namespace std;

// 
template <class T>
struct StackNode {
    T data;
    StackNode<T>* next;
    StackNode(const T& d) : data(d), next(nullptr) {}
};

template <class T>
class Stack {
private:
    StackNode<T>* topPtr;
    int count;

public:
    Stack() : topPtr(nullptr), count(0) {}

    ~Stack() {
        while (!isEmpty()) pop();
    }

    void push(const T& data) {
        StackNode<T>* newNode = new StackNode<T>(data);
        newNode->next = topPtr;
        topPtr = newNode;
        count++;
    }

    bool pop(T& result) {
        if (isEmpty()) return false;
        StackNode<T>* toDelete = topPtr;
        result = toDelete->data;
        topPtr = topPtr->next;
        delete toDelete;
        count--;
        return true;
    }

    void pop() {
        if (isEmpty()) return;
        StackNode<T>* toDelete = topPtr;
        topPtr = topPtr->next;
        delete toDelete;
        count--;
    }

    
    bool top(T& result) const {
        if (isEmpty()) return false;
        result = topPtr->data;
        return true;
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
};

#endif