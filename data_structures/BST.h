#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>

using namespace std;

template <class T>
struct BSTNode {
    string key;
    T data;
    BSTNode<T> *left;
    BSTNode<T> *right;

    BSTNode(const string& k, const T& d);
};

template <class T>
class BST {
private:
    BSTNode<T>* root;

    BSTNode<T>* insertRec(BSTNode<T>* node, const string& key, const T& data);

    BSTNode<T>* searchRec(BSTNode<T>* node, const string& key) const;
    BSTNode<T>* findMin(BSTNode<T>* node) const;

    BSTNode<T>* removeRec(BSTNode<T>* node, const string& key);

    template <class Func>
    void inorderRec(BSTNode<T>* node, Func visit) const;

    void destroyRec(BSTNode<T>* node);

public:
    BST();
    ~BST();

    void insert(const string& key, const T& data);

    bool search(const string& key, T& result) const;

    void remove(const string& key);

    template <class Func>
    void inorderTraverse(Func visit) const;

    bool isEmpty() const;
};

template <class T>
BSTNode<T>::BSTNode(const string& k, const T& d)
    : key(k), data(d), left(nullptr), right(nullptr) {}

template <class T>
BSTNode<T>* BST<T>::insertRec(BSTNode<T>* node, const string& key, const T& data) {
    if (node == nullptr) return new BSTNode<T>(key, data);
    if (key < node->key)
        node->left = insertRec(node->left, key, data);
    else if (key > node->key)
        node->right = insertRec(node->right, key, data);
    else
        node->data = data;
    return node;
}

template <class T>
BSTNode<T>* BST<T>::searchRec(BSTNode<T>* node, const string& key) const {
    if (node == nullptr || node->key == key) return node;
    if (key < node->key) return searchRec(node->left, key);
    return searchRec(node->right, key);
}

template <class T>
BSTNode<T>* BST<T>::findMin(BSTNode<T>* node) const {
    while (node && node->left != nullptr) node = node->left;
    return node;
}

template <class T>
BSTNode<T>* BST<T>::removeRec(BSTNode<T>* node, const string& key) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        node->left = removeRec(node->left, key);
    } else if (key > node->key) {
        node->right = removeRec(node->right, key);
    } else {
        if (node->left == nullptr) {
            BSTNode<T>* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode<T>* temp = node->left;
            delete node;
            return temp;
        }
        BSTNode<T>* successor = findMin(node->right);
        node->key = successor->key;
        node->data = successor->data;
        node->right = removeRec(node->right, successor->key);
    }
    return node;
}

template <class T>
template <class Func>
void BST<T>::inorderRec(BSTNode<T>* node, Func visit) const {
    if (node == nullptr) return;
    inorderRec(node->left, visit);
    visit(node->data);
    inorderRec(node->right, visit);
}

template <class T>
void BST<T>::destroyRec(BSTNode<T>* node) {
    if (node == nullptr) return;
    destroyRec(node->left);
    destroyRec(node->right);
    delete node;
}

template <class T>
BST<T>::BST() : root(nullptr) {}

template <class T>
BST<T>::~BST() { destroyRec(root); }

template <class T>
void BST<T>::insert(const string& key, const T& data) {
    root = insertRec(root, key, data);
}

template <class T>
bool BST<T>::search(const string& key, T& result) const {
    BSTNode<T>* node = searchRec(root, key);
    if (node == nullptr) return false;
    result = node->data;
    return true;
}

template <class T>
void BST<T>::remove(const string& key) {
    root = removeRec(root, key);
}

template <class T>
template <class Func>
void BST<T>::inorderTraverse(Func visit) const {
    inorderRec(root, visit);
}

template <class T>
bool BST<T>::isEmpty() const { return root == nullptr; }

#endif
