#ifndef BST_H
#define BST_H

#include <iostream>
using namespace std;

// ============ CẤU TRÚC NODE CỦA BST ============
template <typename K, typename V>
struct BSTNode {
    K key;              // Khóa tìm kiếm (CCCD)
    V value;            // Dữ liệu (Tenant)
    BSTNode<K, V>* left;    // Con trái (key nhỏ hơn)
    BSTNode<K, V>* right;   // Con phải (key lớn hơn)

    BSTNode(K key, V value) {
        this->key = key;
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

// ============ BST CLASS ============
template <typename K, typename V>
class BST {
private:
    BSTNode<K, V>* root;    // Gốc cây
    int size;               // Số phần tử

    // --- Hàm đệ quy (private, người dùng không gọi trực tiếp) ---
    BSTNode<K, V>* insertHelper(BSTNode<K, V>* node, K key, V value);
    BSTNode<K, V>* searchHelper(BSTNode<K, V>* node, K key);
    BSTNode<K, V>* removeHelper(BSTNode<K, V>* node, K key, bool &success);
    BSTNode<K, V>* findMin(BSTNode<K, V>* node);
    void inorderHelper(BSTNode<K, V>* node);
    void destroyTree(BSTNode<K, V>* node);

public:
    BST();
    ~BST();

    // === THAO TÁC CHÍNH ===
    void insert(K key, V value);    // Thêm phần tử
    V* search(K key);               // Tìm kiếm, trả về con trỏ tới value (nullptr nếu không thấy)
    bool remove(K key);             // Xóa phần tử theo key
    void inorderTraversal();        // Duyệt cây theo thứ tự tăng dần

    // === TIỆN ÍCH ===
    int getSize() const;
    bool isEmpty() const;
};

// ====================================================================
// ĐỊNH NGHĨA CÁC HÀM
// ====================================================================

// --- Constructor ---
template <typename K, typename V>
BST<K, V>::BST() {
    root = nullptr;
    size = 0;
}

// --- Destructor ---
template <typename K, typename V>
BST<K, V>::~BST() {
    destroyTree(root);
}

// Xóa toàn bộ cây (đệ quy: xóa con trước, xóa cha sau)
template <typename K, typename V>
void BST<K, V>::destroyTree(BSTNode<K, V>* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// ============ INSERT ============
// Public: người dùng gọi hàm này
template <typename K, typename V>
void BST<K, V>::insert(K key, V value) {
    root = insertHelper(root, key, value);
    size++;
}

// Private: đệ quy tìm vị trí đúng rồi chèn
template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::insertHelper(BSTNode<K, V>* node, K key, V value) {
    // Tìm được vị trí trống → tạo node mới
    if (node == nullptr) {
        return new BSTNode<K, V>(key, value);
    }

    if (key < node->key) {
        // Key nhỏ hơn → đi sang trái
        node->left = insertHelper(node->left, key, value);
    } else if (key > node->key) {
        // Key lớn hơn → đi sang phải
        node->right = insertHelper(node->right, key, value);
    } else {
        // Key trùng → cập nhật value (CCCD trùng → cập nhật thông tin khách)
        node->value = value;
        size--;  // Bù lại vì hàm insert đã size++ nhưng thực tế không thêm mới
    }

    return node;
}

// ============ SEARCH ============
// Public: trả về con trỏ tới value, nullptr nếu không tìm thấy
template <typename K, typename V>
V* BST<K, V>::search(K key) {
    BSTNode<K, V>* result = searchHelper(root, key);
    if (result != nullptr) {
        return &(result->value);    // Trả về địa chỉ của value
    }
    return nullptr;
}

// Private: đệ quy tìm kiếm
template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::searchHelper(BSTNode<K, V>* node, K key) {
    // Không tìm thấy
    if (node == nullptr) return nullptr;

    // Tìm thấy!
    if (key == node->key) return node;

    // Key nhỏ hơn → tìm bên trái
    if (key < node->key) {
        return searchHelper(node->left, key);
    }

    // Key lớn hơn → tìm bên phải
    return searchHelper(node->right, key);
}

// ============ REMOVE ============
// Public
template <typename K, typename V>
bool BST<K, V>::remove(K key) {
    bool success = false;
    root = removeHelper(root, key, success);
    if (success) size--;
    return success;
}

// Tìm node có key nhỏ nhất (dùng cho xóa node có 2 con)
template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::findMin(BSTNode<K, V>* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Private: đệ quy xóa
template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::removeHelper(BSTNode<K, V>* node, K key, bool &success) {
    if (node == nullptr) {
        success = false;
        return nullptr;
    }

    if (key < node->key) {
        node->left = removeHelper(node->left, key, success);
    } else if (key > node->key) {
        node->right = removeHelper(node->right, key, success);
    } else {
        // Tìm thấy node cần xóa!
        success = true;

        // Trường hợp 1: Node lá (không có con)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        // Trường hợp 2: Có 1 con phải
        else if (node->left == nullptr) {
            BSTNode<K, V>* temp = node->right;
            delete node;
            return temp;
        }
        // Trường hợp 3: Có 1 con trái
        else if (node->right == nullptr) {
            BSTNode<K, V>* temp = node->left;
            delete node;
            return temp;
        }
        // Trường hợp 4: Có 2 con
        // → Tìm node nhỏ nhất bên phải (successor) thay thế
        else {
            BSTNode<K, V>* successor = findMin(node->right);
            node->key = successor->key;
            node->value = successor->value;
            node->right = removeHelper(node->right, successor->key, success);
        }
    }
    return node;
}

// ============ INORDER TRAVERSAL ============
// Duyệt: trái → gốc → phải → kết quả theo thứ tự tăng dần của key
template <typename K, typename V>
void BST<K, V>::inorderTraversal() {
    if (isEmpty()) {
        cout << "(Cay rong)" << endl;
        return;
    }
    inorderHelper(root);
}

template <typename K, typename V>
void BST<K, V>::inorderHelper(BSTNode<K, V>* node) {
    if (node == nullptr) return;

    inorderHelper(node->left);          // Duyệt trái
    cout << "[Key: " << node->key << "]" << endl;
    node->value.display();              // Hiển thị value
    cout << endl;
    inorderHelper(node->right);         // Duyệt phải
}

// ============ TIỆN ÍCH ============
template <typename K, typename V>
int BST<K, V>::getSize() const {
    return size;
}

template <typename K, typename V>
bool BST<K, V>::isEmpty() const {
    return size == 0;
}

#endif // BST_H