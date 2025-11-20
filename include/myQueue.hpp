// Copyright message
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

template <typename T>
class myQueue {
 public:
    explicit myQueue(int initialCapacity = 4);
    ~myQueue();
    void clean();

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    void push(const T& value, const std::string& filename);
    void pop(const std::string& filename);
    void print() const;
    int getSize() const { return size; }

 private:
    T* data;
    int head;
    int tail;
    int size;
    int capacity;
};


template <typename T>
myQueue<T>::myQueue(int initialCapacity)
    : data(nullptr), head(0), tail(0), size(0) {

    if (initialCapacity <= 0) {
        initialCapacity = 4;
    }

    capacity = initialCapacity;
    data = new T[capacity];
}

template <typename T>
myQueue<T>::~myQueue() {
    clean();
}

template <typename T>
void myQueue<T>::clean() {
    delete[] data;
    data = nullptr;
    head = tail = size = capacity = 0;
}

template <typename T>
void myQueue<T>::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);

    if (!in.is_open()) {
        clean();
        capacity = 4;
        data = new T[capacity];
        return;
    }

    std::vector<T> tmp;
    T value;

    while (std::getline(in, value)) {
        tmp.push_back(value);
    }

    in.close();

    clean();

    capacity = 4;
    while (capacity < static_cast<int>(tmp.size())) {
        capacity *= 2;
    }

    data = new T[capacity];
    size = static_cast<int>(tmp.size());
    head = 0;
    tail = size;

    for (int i = 0; i < size; ++i) {
        data[i] = tmp[i];
    }
}

template <typename T>
void myQueue<T>::saveToFile(const std::string& filename) const {
    std::ofstream out(filename, std::ios::trunc);

    if (!out.is_open()) {
        throw std::runtime_error("Cannot open queue file!");
    }

    for (int i = 0; i < size; ++i) {
        out << data[(head + i) % capacity] << "\n";
    }
}

template <typename T>
void myQueue<T>::push(const T& value, const std::string& filename) {
    loadFromFile(filename);

    if (size == capacity) {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size; ++i) {
            newData[i] = data[(head + i) % capacity];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
        head = 0;
        tail = size;
    }

    data[tail] = value;
    tail = (tail + 1) % capacity;
    size++;

    saveToFile(filename);
}

template <typename T>
void myQueue<T>::pop(const std::string& filename) {
    loadFromFile(filename);

    if (size == 0) {
        throw std::underflow_error("Queue is empty!");
    }

    head = (head + 1) % capacity;
    size--;

    saveToFile(filename);
}

template <typename T>
void myQueue<T>::print() const {
    if (size == 0) {
        std::cout << "Queue is empty!" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        std::cout << data[(head + i) % capacity] << " ";
    }

    std::cout << std::endl;
}
