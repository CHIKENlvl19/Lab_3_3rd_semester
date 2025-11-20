// Copyright message
#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class CompleteBinaryTree {
 public:
    CompleteBinaryTree();
    ~CompleteBinaryTree();

    void insert(const T& value);
    void removeLast();
    T getRoot() const;
    int getSize() const;
    void print() const;

 private:
    T* data;
    int size;
    int capacity;

    void resize(int newCapacity);
};


template <typename T>
CompleteBinaryTree<T>::CompleteBinaryTree()
    : data(nullptr), size(0), capacity(4) {
    data = new T[capacity];
}

template <typename T>
CompleteBinaryTree<T>::~CompleteBinaryTree() {
    delete[] data;
}

template <typename T>
void CompleteBinaryTree<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void CompleteBinaryTree<T>::insert(const T& value) {
    if (size >= capacity) {
        resize(capacity * 2);
    }

    data[size++] = value;
}

template <typename T>
void CompleteBinaryTree<T>::removeLast() {
    if (size == 0) {
        throw std::underflow_error("Tree is empty, nothing to remove");
    }

    size--;
}

template <typename T>
T CompleteBinaryTree<T>::getRoot() const {
    if (size == 0) {
        throw std::underflow_error("Tree is empty, no root exists");
    }

    return data[0];
}

template <typename T>
int CompleteBinaryTree<T>::getSize() const {
    return size;
}

template <typename T>
void CompleteBinaryTree<T>::print() const {
    std::cout << "[";
    for (int i = 0; i < size; i++) {
        std::cout << data[i];
        if (i != size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
