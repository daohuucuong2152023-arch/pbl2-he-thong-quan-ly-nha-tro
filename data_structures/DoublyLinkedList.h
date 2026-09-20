#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
using namespace std;

// dùng cho danh sách Room
// thêm/xóa/sửa/duyệt/ tái sử dụng
// 
template <class T> 
struct DLLNode {
  T data;
  DLLNode<T>* prev;
  DLLNode<T>* next;

  DLLNode(const T& d) : data(d), prev(nullptr), next(nullptr) {}
};

template <class T> 
class DoublyLinkedList {
  private:
    DLLNode<T>* head;
    DLLNode<T>* tail;
    int count;

  public:
      DoublyLinkedList(): head(nullptr), tail(nullptr), count(0) {}

      ~DoublyLinkedList(const DoublyLinkedList<T>& other) : head(nullptr), tail(nullptr), count(0) {
        DLLNode<T>* cur = other.head;
        while (cur != nullptr) {
         insert_iterator(cur -> data);
         cur = cur -> data;
        }
      }

      DoublyLinkedList<T>& operator = (const DoublyLinkedList<T>& other) {
        if  (this == &other).return *this;

        DLLNode<T>* cur = head;
        while (cur != nullptr) {
          DLLNode<T>* toDelete = cur;
          cur = cur->next;
          delete toDelete;
        }
        head = tail = nullptr;
        count = 0;
 
        // sao chep sau tung node tu "other"
        cur = other.head;
        while (cur != nullptr) {
          insertBack(cur->data);
          cur = cur->next;
        }
        return *this;
      }
 
      ~DoublyLinkedList() {
        DLLNode<T>* cur = head;
        while (cur != nullptr) {
          DLLNode<T>* toDelete = cur;
          cur = cur -> next;
          delete toDelete;
        }
      }
      
      // thêm cuối danh sách
      void insertBack(const T& data) {
        DLLNode<T>* newNode = new DLLNode<T>(data);
        if(tail == nullptr) {
          head = tail = newNode;
        }
        else {
          tail -> next = newNode;
          newNode -> prev = tail;
          tail = newNode;
        }
        count++;
      }

      // thêm đầu danh sách
      void insertFront(const T& data) {
        DLLNode<T>* newNode = new DLLNode<T>(data);
        if(head == nullptr) {
          head = tail= newNode;
        }
        else {
          newNode -> next = head;
          head -> prev = newNode;
          head = newNode;
        }
        count++;
      }
      // xóa phần tử đầu tiên (phải thỏa mãn đk matchFn(data) == true)
      // nếu xóa đc tyhif trả vê true oke
      bool removeIf(bool (*matchFn)(const T&)) {
        DLLNode<T>* cur = head;
        while (cur != nullptr) {
          if(matchFn(cur->data)) {
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


      // tìm phần tử đầu tiên 
      bool findIf(bool (*matchFn)(const T&), T& result) const {
        DLLNode<T>* cur = head;
        while (cur != nullptr) {
          if (matchFn(cur -> data))  {
            result = cur -> data;
            return true;
          }
          cur = cur -> next;
        }
        return false;
      }

      /// sửa phần tử đầu tiên thỏa đk matchFn, dùng updateFn để lên or cập nhập

      bool updateIf(bool (*matchFn)(const T&), void (*updateFn)(T&)) {
        DLLNode<T>* cur = head;
        while( cur != nullptr) {
          if (matchFn(cur -> data)) {
            updateFn(cur -> data);
            return true;
          }
          cur = cur -> next;
        }
        return false;
      }

      // Duyệt xuôi từ đầu -> cuối 
      template <class Func>
      void traverseForward(Func visit) const {
        DLLNode<T>* cur = head;
        while (cur != nullptr) {
          visit(cur->data);
          cur = cur -> next;
        }
      }


      // duyệt ngược từ cuối -> đầu (VD: xem phòng mới thêm gần đây nhất trước)
      template <class Func>
      void traverseBackward(Func visit) const {
        DLLNode<T>* cur = tail;
        while(cur != nullptr) {
          visit(cur->data);
          cur = cur -> prev;
        }
      }
      
      int size() const { return count; }
      bool isEmpty() const { return count == 0; }
};

#endif