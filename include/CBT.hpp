// Copyright message
#pragma once
#include <stdexcept>
#include <iostream>
#include "../include/myQueue_for_CBT.hpp"

template <typename T>
class CompleteBinaryTree {
 private:
    struct Node {
        T value;
        Node* left;
        Node* right;
        explicit Node(const T& v)
            : value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int size;

 public:
    CompleteBinaryTree() : root(nullptr), size(0) {}

    ~CompleteBinaryTree() { clear(); }

    CompleteBinaryTree(const CompleteBinaryTree& other)
        : root(nullptr), size(0) {
        if (!other.root) {
            return;
        }

        root = new Node(other.root->value);
        size = other.size;

        CBTQueue<Node*> qOld;
        CBTQueue<Node*> qNew;

        qOld.push(other.root);
        qNew.push(root);

        while (!qOld.empty()) {
            Node* o = qOld.front();
            qOld.pop();

            Node* n = qNew.front();
            qNew.pop();

            if (o->left) {
                n->left = new Node(o->left->value);
                qOld.push(o->left);
                qNew.push(n->left);
            }
            if (o->right) {
                n->right = new Node(o->right->value);
                qOld.push(o->right);
                qNew.push(n->right);
            }
        }
    }

    CompleteBinaryTree& operator=(const CompleteBinaryTree& other) {
        if (this == &other) {
            return *this;
        }

        clear();

        if (!other.root) {
            return *this;
        }

        root = new Node(other.root->value);
        size = other.size;

        CBTQueue<Node*> qOld;
        CBTQueue<Node*> qNew;

        qOld.push(other.root);
        qNew.push(root);

        while (!qOld.empty()) {
            Node* o = qOld.front();
            qOld.pop();

            Node* n = qNew.front();
            qNew.pop();

            if (o->left) {
                n->left = new Node(o->left->value);
                qOld.push(o->left);
                qNew.push(n->left);
            }
            if (o->right) {
                n->right = new Node(o->right->value);
                qOld.push(o->right);
                qNew.push(n->right);
            }
        }

        return *this;
    }

 private:
    void clear() {
        if (!root) return;

        CBTQueue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* n = q.front();
            q.pop();

            if (n->left) {
                q.push(n->left);
            }
            if (n->right) {
                q.push(n->right);
            }
            delete n;
        }

        root = nullptr;
        size = 0;
    }

 public:
    int getSize() const {
        return size;
    }

    bool empty() const {
        return size == 0;
    }

    T getRoot() const {
        if (!root) throw std::underflow_error("Tree is empty");
        return root->value;
    }

    void insert(const T& value) {
        Node* newNode = new Node(value);
        size++;

        if (!root) {
            root = newNode;
            return;
        }

        CBTQueue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* cur = q.front();
            q.pop();

            if (!cur->left) {
                cur->left = newNode;
                return;
            }
            if (!cur->right) {
                cur->right = newNode;
                return;
            }

            q.push(cur->left);
            q.push(cur->right);
        }
    }


    void removeLast() {
        if (!root)
            throw std::underflow_error("Tree is empty");

        if (size == 1) {
            delete root;
            root = nullptr;
            size = 0;
            return;
        }

        CBTQueue<Node*> q;
        q.push(root);

        Node* last = nullptr;
        Node* parent = nullptr;

        while (!q.empty()) {
            Node* cur = q.front();
            q.pop();

            if (cur->left) {
                parent = cur;
                last = cur->left;
                q.push(cur->left);
            }
            if (cur->right) {
                parent = cur;
                last = cur->right;
                q.push(cur->right);
            }
        }

        // удаляем последний элемент
        if (parent->right == last) {
            delete parent->right;
            parent->right = nullptr;
        } else {
            delete parent->left;
            parent->left = nullptr;
        }

        size--;
    }


    void printLevelOrder() const {
        if (!root) {
            std::cout << "[]" << std::endl;
            return;
        }

        CBTQueue<Node*> q;
        q.push(root);

        std::cout << "[";

        bool first = true;

        while (!q.empty()) {
            Node* n = q.front();
            q.pop();

            if (!first) {
                std::cout << ", ";
            }
            first = false;

            std::cout << n->value;

            if (n->left) {
                q.push(n->left);
            }
            if (n->right) {
                q.push(n->right);
            }
        }

        std::cout << "]" << std::endl;
    }
};
