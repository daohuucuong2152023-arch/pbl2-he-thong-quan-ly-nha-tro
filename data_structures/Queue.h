#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

// ============ NODE CỦA QUEUE ============
template <typename T>
struct QueueNode {
    T data;
    QueueNode<T>* next;

    QueueNode(T data) {
        this->data = data;
        this->next = nullptr;
    }
};

// ============ QUEUE CLASS ============
template <typename T>
class Queue {
private:
    QueueNode<T>* front;    // Đầu hàng (lấy ra ở đây)
    QueueNode<T>* rear;     // Cuối hàng (thêm vào ở đây)
    int size;

public:
    Queue();
    ~Queue();

<<<<<<< HEAD
    ~Queue() {
      while (!isEmpty()) dequeue();
    }

    // thêm vào cuối hàng đợi (khách vừa đăng kí check in)
    void enqueue(const T& data) {
      QueueNode<T>* newNode = new QueueNode<T>(data);
      if(rearPtr == nullptr) {
        frontPtr = rearPtr = newNode;
      }
      else {
        rearPtr -> next = newNode;
        rearPtr = newNode;
      }
      count++;
    }

    // lấy và xóa phần tử đầu hàng đợi (xử lí yêu cầu sớm nhất)

    bool dequeue(T& result) {
        if (isEmpty()) return false;
        QueueNode<T>* toDelete = frontPtr;
        result = toDelete->data;
        frontPtr = frontPtr->next;
        if (frontPtr == nullptr) rearPtr = nullptr;
        delete toDelete;
        count--;
        return true;
    }
    void dequeue() {
        if (isEmpty()) return;
        QueueNode<T>* toDelete = frontPtr;
        frontPtr = frontPtr->next;
        if (frontPtr == nullptr) rearPtr = nullptr;
        delete toDelete;
        count--;
    }
    // xem phần tử đầu hàng đợi mà không xóa
    bool front(T& result) const {
      if (isEmpty()) return false;
      result = frontPtr -> data;
      return true;
    }

    int size() const { return count; };
    bool isEmpty() const { return count == 0; }
=======
    void enqueue(T data);       // Thêm vào cuối hàng
    bool dequeue();             // Xóa phần tử đầu hàng
    T* peek();                  // Xem phần tử đầu hàng (không xóa)
    int getSize() const;
    bool isEmpty() const;
    void displayAll() const;
>>>>>>> 9f4865fd783b3ba40f21afb8065bcb8b4477af97
};

// ====================================================================
// ĐỊNH NGHĨA CÁC HÀM
// ====================================================================

// --- Constructor ---
template <typename T>
Queue<T>::Queue() {
    front = nullptr;
    rear = nullptr;
    size = 0;
}

// --- Destructor: giải phóng tất cả node ---
template <typename T>
Queue<T>::~Queue() {
    while (!isEmpty()) {
        dequeue();
    }
}

// --- Enqueue: thêm vào cuối hàng ---
template <typename T>
void Queue<T>::enqueue(T data) {
    QueueNode<T>* newNode = new QueueNode<T>(data);

    if (isEmpty()) {
        // Hàng rỗng: node mới vừa là front vừa là rear
        front = newNode;
        rear = newNode;
    } else {
        // Nối node mới vào sau rear
        rear->next = newNode;
        rear = newNode;     // Cập nhật rear
    }
    size++;
}

// --- Dequeue: xóa phần tử đầu hàng ---
template <typename T>
bool Queue<T>::dequeue() {
    if (isEmpty()) {
        return false;
    }

    QueueNode<T>* toDelete = front;
    front = front->next;    // Front mới = node thứ 2

    // Nếu hàng chỉ còn 1 phần tử rồi xóa → rear cũng thành nullptr
    if (front == nullptr) {
        rear = nullptr;
    }

    delete toDelete;
    size--;
    return true;
}

// --- Peek: xem phần tử đầu hàng (không xóa) ---
template <typename T>
T* Queue<T>::peek() {
    if (isEmpty()) {
        return nullptr;
    }
    return &(front->data);
}

// --- Số phần tử ---
template <typename T>
int Queue<T>::getSize() const {
    return size;
}

// --- Kiểm tra rỗng ---
template <typename T>
bool Queue<T>::isEmpty() const {
    return size == 0;
}

// --- Hiển thị tất cả (từ front đến rear) ---
template <typename T>
void Queue<T>::displayAll() const {
    if (isEmpty()) {
        cout << "(Hang doi rong)" << endl;
        return;
    }

    QueueNode<T>* current = front;
    int index = 1;
    while (current != nullptr) {
        cout << "  Thu tu " << index << ":" << endl;
        current->data.display();
        cout << endl;
        current = current->next;
        index++;
    }
}

#endif // QUEUE_H