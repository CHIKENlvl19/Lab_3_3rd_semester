// Copyright message
#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

template <typename T>
class myQueue {
 public:
    explicit myQueue(int initialCapacity = 4)
        : data(nullptr), head(0), tail(0), size(0) {
        if (initialCapacity <= 0) initialCapacity = 4;
        capacity = initialCapacity;
        data = new T[capacity];
    }

    myQueue(const myQueue& other)
        : data(new T[other.capacity]),
          head(0),
          tail(other.size),
          size(other.size),
          capacity(other.capacity) {
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[(other.head + i) % other.capacity];
        }
    }

    myQueue& operator=(const myQueue& other) {
        if (this != &other) {
            delete[] data;

            capacity = other.capacity;
            size = other.size;
            head = 0;
            tail = size;
            data = new T[capacity];

            for (int i = 0; i < size; ++i) {
                data[i] = other.data[(other.head + i) % other.capacity];
            }
        }
        return *this;
    }


    ~myQueue() {
        clean();
    }

    void clean() {
        delete[] data;
        data = nullptr;
        head = tail = size = capacity = 0;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        clean();

        if (!in.is_open()) {
            capacity = 4;
            data = new T[capacity];
            return;
        }

        // cчитаем количество элементов в файле
        int count = 0;
        T temp;
        while (in >> temp) {
            count++;
        }

        capacity = 4;
        while (capacity < count) {
            capacity *= 2;
        }

        data = new T[capacity];
        head = 0;
        size = count;
        tail = count;

        // сброс позиции файла
        in.clear();
        in.seekg(0, std::ios::beg);

        for (int i = 0; i < count; ++i) {
            in >> data[i];
        }

        in.close();
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename, std::ios::trunc);
        if (!out.is_open()) {
            throw std::runtime_error("Cannot open queue file!");
        }

        for (int i = 0; i < size; ++i) {
            out << data[(head + i) % capacity] << "\n";
        }
    }

    void push(const T& value, const std::string& filename) {
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

    void pop(const std::string& filename) {
        loadFromFile(filename);

        if (size == 0) {
            throw std::underflow_error("Queue is empty!");
        }

        head = (head + 1) % capacity;
        size--;

        saveToFile(filename);
    }

    void print() const {
        if (size == 0) {
            std::cout << "Queue is empty!" << std::endl;
            return;
        }

        for (int i = 0; i < size; ++i) {
            std::cout << data[(head + i) % capacity] << " ";
        }
        std::cout << std::endl;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

 private:
    T* data;
    int head;
    int tail;
    int size;
    int capacity;
};
