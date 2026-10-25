#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>
#include "../models/Room.h"

using namespace std;

struct BSTNode {
    string key;
    Room data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(const string& k, const Room& d);
};

class BST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, const string& key, const Room& data);

    BSTNode* searchRec(BSTNode* node, const string& key) const;
    BSTNode* findMin(BSTNode* node) const;

    BSTNode* removeRec(BSTNode* node, const string& key);

    template <class Func>
    void inorderRec(BSTNode* node, Func visit) const;

    void destroyRec(BSTNode* node);

public:
    BST();
    ~BST();

    void insert(const string& key, const Room& data);

    bool search(const string& key, Room& result) const;

    void remove(const string& key);

    template <class Func>
    void inorderTraverse(Func visit) const;

    bool isEmpty() const;
};

inline BSTNode::BSTNode(const string& k, const Room& d)
    : key(k), data(d), left(nullptr), right(nullptr) {}

inline BSTNode* BST::insertRec(BSTNode* node, const string& key, const Room& data) {
    if (node == nullptr) return new BSTNode(key, data);
    if (key < node->key)
        node->left = insertRec(node->left, key, data);
    else if (key > node->key)
        node->right = insertRec(node->right, key, data);
    else
        node->data = data;
    return node;
}

inline BSTNode* BST::searchRec(BSTNode* node, const string& key) const {
    if (node == nullptr || node->key == key) return node;
    if (key < node->key) return searchRec(node->left, key);
    return searchRec(node->right, key);
}

inline BSTNode* BST::findMin(BSTNode* node) const {
    while (node && node->left != nullptr) node = node->left;
    return node;
}

inline BSTNode* BST::removeRec(BSTNode* node, const string& key) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        node->left = removeRec(node->left, key);
    } else if (key > node->key) {
        node->right = removeRec(node->right, key);
    } else {
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }
        BSTNode* successor = findMin(node->right);
        node->key = successor->key;
        node->data = successor->data;
        node->right = removeRec(node->right, successor->key);
    }
    return node;
}

template <class Func>
void BST::inorderRec(BSTNode* node, Func visit) const {
    if (node == nullptr) return;
    inorderRec(node->left, visit);
    visit(node->data);
    inorderRec(node->right, visit);
}

inline void BST::destroyRec(BSTNode* node) {
    if (node == nullptr) return;
    destroyRec(node->left);
    destroyRec(node->right);
    delete node;
}

inline BST::BST() : root(nullptr) {}

inline BST::~BST() { destroyRec(root); }

inline void BST::insert(const string& key, const Room& data) {
    root = insertRec(root, key, data);
}

inline bool BST::search(const string& key, Room& result) const {
    BSTNode* node = searchRec(root, key);
    if (node == nullptr) return false;
    result = node->data;
    return true;
}

inline void BST::remove(const string& key) {
    root = removeRec(root, key);
}

template <class Func>
void BST::inorderTraverse(Func visit) const {
    inorderRec(root, visit);
}

inline bool BST::isEmpty() const { return root == nullptr; }

#endif