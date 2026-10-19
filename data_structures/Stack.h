#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

// ============ NODE CỦA STACK ============
template <typename T>
struct StackNode {
    T data;
    StackNode<T>* next;

    StackNode(T data) {
        this->data = data;
        this->next = nullptr;
    }
};

// ============ STACK CLASS ============
template <typename T>
class Stack {
private:
    StackNode<T>* top;      // Đỉnh stack
    int size;

public:
    Stack();
    ~Stack();

    void push(T data);      // Đẩy phần tử lên đỉnh
    bool pop();             // Xóa phần tử đỉnh
    T* peek();              // Xem phần tử đỉnh (không xóa)
    int getSize() const;
    bool isEmpty() const;
    void displayAll() const;
};

// ====================================================================
// ĐỊNH NGHĨA CÁC HÀM
// ====================================================================

// --- Constructor ---
template <typename T>
Stack<T>::Stack() {
    top = nullptr;
    size = 0;
}

// --- Destructor ---
template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

// --- Push: thêm lên đỉnh ---
template <typename T>
void Stack<T>::push(T data) {
    StackNode<T>* newNode = new StackNode<T>(data);
    newNode->next = top;    // Node mới trỏ tới top cũ
    top = newNode;          // Cập nhật top = node mới
    size++;
}

// --- Pop: xóa phần tử đỉnh ---
template <typename T>
bool Stack<T>::pop() {
    if (isEmpty()) {
        return false;
    }

    StackNode<T>* toDelete = top;
    top = top->next;        // Top mới = node bên dưới
    delete toDelete;
    size--;
    return true;
}

// --- Peek: xem đỉnh (không xóa) ---
template <typename T>
T* Stack<T>::peek() {
    if (isEmpty()) {
        return nullptr;
    }
    return &(top->data);
}

// --- Số phần tử ---
template <typename T>
int Stack<T>::getSize() const {
    return size;
}

// --- Kiểm tra rỗng ---
template <typename T>
bool Stack<T>::isEmpty() const {
    return size == 0;
}

// --- Hiển thị tất cả (từ đỉnh xuống đáy) ---
template <typename T>
void Stack<T>::displayAll() const {
    if (isEmpty()) {
        cout << "(Stack rong)" << endl;
        return;
    }

    StackNode<T>* current = top;
    int index = 1;
    cout << "  [DINH]" << endl;
    while (current != nullptr) {
        cout << "  " << index << ". ";
        current->data.display();
        cout << endl;
        current = current->next;
        index++;
    }
    cout << "  [DAY]" << endl;
}

#endif // STACK_H