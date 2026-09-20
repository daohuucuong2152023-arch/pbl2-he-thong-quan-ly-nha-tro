// class ở trên, định nghĩa hàm ở dưới để include cho khỏe

#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>
#include "../models/Room.h"

using namespace std;

struct BSTNode{
  string key;
  Room data; // Dữ liệu toàn bộ phòng 
  BSTNode *left;
  BSTNode *right;

  // Hàm khởi tạo(Constructor)
  BSTNode(const string& k, const Room& d)
        : key(k), data(d), left(nullptr), right(nullptr) {}
};

class BST {
  private:
    BSTNode* root; 

    // Them
    BSTNode* insertRec(BSTNode* node, const string& key, const Room& data) {
        if (node == nullptr) return new BSTNode(key, data);
        if (key < node->key)
            node->left = insertRec(node->left, key, data);
        else if (key > node->key)
            node->right = insertRec(node->right, key, data);
        else
            node->data = data;
        return node;
    }

    //tim
    BSTNode* searchRec(BSTNode* node, const string& key) const {
        if (node == nullptr || node->key == key) return node;
        if (key < node->key) return searchRec(node->left, key);
        return searchRec(node->right, key);
    }

    BSTNode* findMin(BSTNode* node) const {
        while (node && node->left != nullptr) node = node->left;
        return node;
    }

    // xoa
    BSTNode* removeRec(BSTNode* node, const string& key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = removeRec(node->left, key);
        } else if (key > node->key) {
            node->right = removeRec(node->right, key);
        } else {
            // thấy node cần xóa
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            // node có 2 con
            BSTNode* successor = findMin(node->right);
            node->key = successor->key;
            node->data = successor->data;
            node->right = removeRec(node->right, successor->key);
        }
        return node;
    }

    // Duyệt cây theo thứ tự tăng dần
    template <class Func>
    void inorderRec(BSTNode* node, Func visit) const {
        if (node == nullptr) return;
        inorderRec(node->left, visit);
        visit(node->data);
        inorderRec(node->right, visit);
    }

    
    void destroyRec(BSTNode* node) {
        if (node == nullptr) return;
        destroyRec(node->left);
        destroyRec(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { destroyRec(root); } // <-- SUA: hoan thanh destructor, giai phong toan bo cay

    // Thêm phòng mới (hoặc cập nhật nếu mã phòng đã tồn tại)
    void insert(const string& key, const Room& data) {
        root = insertRec(root, key, data);
    }

    // Tìm theo mã phòng. Trả về true nếu tìm thấy, kết quả ghi vào 'result'
    bool search(const string& key, Room& result) const {
        BSTNode* node = searchRec(root, key);
        if (node == nullptr) return false;
        result = node->data;
        return true;
    }

    // Xóa theo mã phòng
    void remove(const string& key) {
        root = removeRec(root, key);
    }

    // Duyệt inorder -> danh sách sắp theo mã phòng tăng dần
    void inorderTraverse(void (*visit)(const Room&)) const {
        inorderRec(root, visit);
    }

    bool isEmpty() const { return root == nullptr; }
};

#endif