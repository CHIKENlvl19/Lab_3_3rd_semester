// Copyright message
#pragma once

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class myArray {
 public:
    explicit myArray(int initialCapacity = 4);
    myArray(const myArray& other);
    myArray<T>& operator=(const myArray& other);
    ~myArray();

    void push_back(T value);
    void insert(int index, T value);
    T& getElement(int index);
    void remove(int index);
    void swapElements(int index, T value);
    int getSize() const;
    void print() const;

 private:
    void clean();

    T* data;
    int size;
    int capacity;
};


template <typename T>
myArray<T>::myArray(int initialCapacity)
    : data(nullptr), size(0), capacity(0) {
    if (initialCapacity <= 0)
        initialCapacity = 4;

    data = new T[initialCapacity];
    capacity = initialCapacity;
}

template <typename T>
myArray<T>::myArray(const myArray& other)
    : data(new T[other.capacity]),
      size(other.size),
      capacity(other.capacity) {
    for (int i = 0; i < size; ++i)
        data[i] = other.data[i];
}

template <typename T>
myArray<T>& myArray<T>::operator=(const myArray& other) {
    if (this == &other)
        return *this;

    T* newData = new T[other.capacity];
    for (int i = 0; i < other.size; ++i)
        newData[i] = other.data[i];

    delete[] data;
    data = newData;
    size = other.size;
    capacity = other.capacity;

    return *this;
}

template <typename T>
myArray<T>::~myArray() {
    clean();
}

template <typename T>
void myArray<T>::clean() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

template <typename T>
void myArray<T>::push_back(T value) {
    if (size >= capacity) {
        int newCapacity = capacity > 0 ? capacity * 2 : 4;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size; ++i)
            newData[i] = data[i];

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    data[size++] = value;
}

template <typename T>
void myArray<T>::insert(int index, T value) {
    if (index < 0 || index > size)
        throw out_of_range("Invalid index");

    if (size >= capacity) {
        int newCapacity = capacity > 0 ? capacity * 2 : 4;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size; ++i)
            newData[i] = data[i];

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    for (int i = size; i > index; --i)
        data[i] = data[i - 1];

    data[index] = value;
    size++;
}

template <typename T>
T& myArray<T>::getElement(int index) {
    if (index < 0 || index >= size)
        throw out_of_range("Invalid index");

    return data[index];
}

template <typename T>
void myArray<T>::remove(int index) {
    if (index < 0 || index >= size)
        throw out_of_range("Invalid index");

    for (int i = index; i < size - 1; ++i)
        data[i] = data[i + 1];

    size--;
}

template <typename T>
void myArray<T>::swapElements(int index, T value) {
    if (index < 0 || index >= size)
        throw out_of_range("Invalid index");

    data[index] = value;
}

template <typename T>
int myArray<T>::getSize() const {
    return size;
}

template <typename T>
void myArray<T>::print() const {
    cout << "[";
    for (int i = 0; i < size; ++i) {
        cout << data[i];
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << "]\n";
}
