// Copyright message
#pragma once

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
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

    // вспомогательная функция: найти путь к узлу по индексу
    // возвращает родителя и направление (true = right, false = left)
    Node* findParent(int index, bool& goRight) {
        if (index <= 1) return nullptr;

        // путь через двоичное представление
        int path = index;
        int depth = 0;

        // глубина (количество битов - 1)
        while (path > 1) {
            path >>= 1;
            depth++;
        }

        // идем по дереву, используя биты как направления
        Node* current = root;
        for (int level = depth - 1; level > 0; --level) {
            if (index & (1 << level)) {
                current = current->right;
            } else {
                current = current->left;
            }
        }

        goRight = (index & 1);  // последний бит определяет left/right
        return current;
    }

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

    // O(log n)
    void insert(const T& value) {
        Node* newNode = new Node(value);
        size++;

        if (!root) {
            root = newNode;
            return;
        }

        // находим родителя для нового узла
        bool goRight;
        Node* parent = findParent(size, goRight);

        if (goRight) {
            parent->right = newNode;
        } else {
            parent->left = newNode;
        }
    }

    // O(log n)
    void removeLast() {
        if (!root) {
            throw std::underflow_error("Tree is empty");
        }

        if (size == 1) {
            delete root;
            root = nullptr;
            size = 0;
            return;
        }

        // находим родителя последнего узла
        bool goRight;
        Node* parent = findParent(size, goRight);

        // удаляем последний узел
        if (goRight) {
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

    // текстовый формат
    void saveText(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }

        file << size << "\n";

        if (root) {
            CBTQueue<Node*> q;
            q.push(root);
            bool first = true;

            while (!q.empty()) {
                Node* node = q.front();
                q.pop();

                if (!first) file << " ";
                first = false;
                file << node->value;

                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        file.close();
    }

    void loadText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }

        clear();

        int newSize;
        file >> newSize;

        for (int i = 0; i < newSize; ++i) {
            T value;
            file >> value;
            insert(value);
        }
        file.close();
    }

    // бинарный формат
    void saveBinary(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }

        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        if (root) {
            CBTQueue<Node*> q;
            q.push(root);

            while (!q.empty()) {
                Node* node = q.front();
                q.pop();

                file.write(reinterpret_cast<const char*>(&node->value)
                    , sizeof(T));

                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        file.close();
    }

    void loadBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }

        clear();

        int newSize;
        file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));

        for (int i = 0; i < newSize; ++i) {
            T value;
            file.read(reinterpret_cast<char*>(&value), sizeof(T));
            insert(value);
        }
        file.close();
    }
};
