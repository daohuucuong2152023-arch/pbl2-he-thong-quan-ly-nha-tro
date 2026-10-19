#ifndef QUEUE_H
#define QUEUE_H
using namespace std;

// chứa khách đang đợi chờ xử lí check - in

template <class T>
struct QueueNode {
  T data;
  QueueNode<T>* next;
  QueueNode(const T& d) : data(d), next(nullptr) {}
};

template <class T>
class Queue {
  private:
    QueueNode<T>* frontPtr;
    QueueNode<T>* rearPtr;
    int count;

  public:
    Queue() : frontPtr(nullptr), rearPtr(nullptr), count(0) {}

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
      if(isEmpty()) return false;
      QueueNode<T>* toDelete = frontPtr;
      result = toDelete -> data;
      frontPtr = frontPtr->next;
      if(frontPtr == nullptr) rearPtr = nullptr;
      delete toDelete;
      count--;
      return true;
    }

    // dữ nguyên dữ liệu (dùng trong nội bộ trong destructor)
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
};

#endif


