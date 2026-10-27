#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
using namespace std;


template <typename T>
struct Node {
    T data;             // Dữ liệu (Room, Tenant, Invoice...)
    Node<T>* prev;      // Con trỏ tới node trước
    Node<T>* next;      // Con trỏ tới node sau

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
    Node<T>* head;      // Con trỏ tới node đầu tiên
    Node<T>* tail;      // Con trỏ tới node cuối cùng
    int size;           // Số phần tử hiện có

public:
    // === CONSTRUCTOR & DESTRUCTOR ===
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

    // === THÊM PHẦN TỬ ===
    void addFront(T data);      // Thêm vào đầu
    void addBack(T data);       // Thêm vào cuối

    // === XÓA PHẦN TỬ ===
    bool removeAt(int index);   // Xóa phần tử tại vị trí index

    // === TRUY CẬP ===
    Node<T>* getHead() const;
    Node<T>* getTail() const;
    Node<T>* getNodeAt(int index) const;    // Lấy node tại vị trí index
    int getSize() const;
    bool isEmpty() const;

    // === HIỂN THỊ ===
    void displayAll() const;    // Hiển thị tất cả phần tử
};

// ====================================================================
// ĐỊNH NGHĨA CÁC HÀM (phải nằm trong .h vì là template)
// ====================================================================

// --- Constructor ---
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// --- Destructor: giải phóng bộ nhớ tất cả node ---
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* nextNode = current->next;  // Lưu lại node tiếp theo
        delete current;                      // Xóa node hiện tại
        current = nextNode;                  // Chuyển sang node tiếp
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// --- Thêm vào đầu danh sách ---
template <typename T>
void DoublyLinkedList<T>::addFront(T data) {
    Node<T>* newNode = new Node<T>(data);

    if (isEmpty()) {
        // Danh sách rỗng: node mới vừa là head vừa là tail
        head = newNode;
        tail = newNode;
    } else {
        // Nối node mới vào trước head cũ
        newNode->next = head;   // Node mới trỏ tới head cũ
        head->prev = newNode;   // Head cũ trỏ ngược lại node mới
        head = newNode;         // Cập nhật head = node mới
    }
    size++;
}

// --- Thêm vào cuối danh sách ---
template <typename T>
void DoublyLinkedList<T>::addBack(T data) {
    Node<T>* newNode = new Node<T>(data);

    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        // Nối node mới vào sau tail cũ
        newNode->prev = tail;   // Node mới trỏ ngược lại tail cũ
        tail->next = newNode;   // Tail cũ trỏ tới node mới
        tail = newNode;         // Cập nhật tail = node mới
    }
    size++;
}

// --- Xóa phần tử tại vị trí index (0-based) ---
template <typename T>
bool DoublyLinkedList<T>::removeAt(int index) {
    // Kiểm tra index hợp lệ
    if (index < 0 || index >= size) {
        return false;
    }

    Node<T>* toDelete = getNodeAt(index);
    if (toDelete == nullptr) return false;

    // Trường hợp 1: Xóa node duy nhất trong danh sách
    if (size == 1) {
        head = nullptr;
        tail = nullptr;
    }
    // Trường hợp 2: Xóa node đầu (head)
    else if (toDelete == head) {
        head = head->next;      // Head mới = node thứ 2
        head->prev = nullptr;   // Head mới không có node trước
    }
    // Trường hợp 3: Xóa node cuối (tail)
    else if (toDelete == tail) {
        tail = tail->prev;      // Tail mới = node áp cuối
        tail->next = nullptr;   // Tail mới không có node sau
    }
    // Trường hợp 4: Xóa node ở giữa
    else {
        toDelete->prev->next = toDelete->next;  // Node trước trỏ qua node sau
        toDelete->next->prev = toDelete->prev;  // Node sau trỏ ngược lại node trước
    }

    delete toDelete;    // Giải phóng bộ nhớ
    size--;
    return true;
}

// --- Lấy node đầu ---
template <typename T>
Node<T>* DoublyLinkedList<T>::getHead() const {
    return head;
}

// --- Lấy node cuối ---
template <typename T>
Node<T>* DoublyLinkedList<T>::getTail() const {
    return tail;
}

// --- Lấy node tại vị trí index (0-based) ---
template <typename T>
Node<T>* DoublyLinkedList<T>::getNodeAt(int index) const {
    if (index < 0 || index >= size) {
        return nullptr;
    }

    Node<T>* current;

    // Tối ưu: nếu index gần đầu thì duyệt từ head, gần cuối thì duyệt từ tail
    if (index <= size / 2) {
        // Duyệt từ đầu
        current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        // Duyệt từ cuối (ưu điểm của Doubly so với Singly!)
        current = tail;
        for (int i = size - 1; i > index; i--) {
            current = current->prev;
        }
    }

    return current;
}

// --- Số phần tử ---
template <typename T>
int DoublyLinkedList<T>::getSize() const {
    return size;
}

// --- Kiểm tra rỗng ---
template <typename T>
bool DoublyLinkedList<T>::isEmpty() const {
    return size == 0;
}

// --- Hiển thị tất cả phần tử ---
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
        current->data.display();    // Gọi hàm display() của T (Room, Tenant...)
        cout << endl;
        current = current->next;
        index++;
    }
}

#endif // DOUBLYLINKEDLIST_H
