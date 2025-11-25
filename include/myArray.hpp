// Copyright message
#pragma once

#include <iostream>
#include <stdexcept>


template <typename T>
class myArray {
 public:
    explicit myArray(int initialCapacity = 4)
        : data(nullptr), size(0), capacity(0) {
        if (initialCapacity <= 0) {
            initialCapacity = 4;
        }

        data = new T[initialCapacity];
        capacity = initialCapacity;
    }

    myArray(const myArray& other)
        : data(new T[other.capacity]),
        size(other.size),
        capacity(other.capacity) {
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }


    myArray<T>& operator=(const myArray& other) {
        if (this == &other) {
            (void)this;
            return *this;
        }

        T* newData = new T[other.capacity];
        for (int i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }

        delete[] data;
        data = newData;
        size = other.size;
        capacity = other.capacity;

        return *this;
    }

    T& operator[](const int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Invalid index");
        }

        return data[index];
    }

    void clean() {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    ~myArray() {
        clean();
    }


    void push_back(T value) {
        if (size >= capacity) {
            int newCapacity = capacity > 0 ? capacity * 2 : 4;
            T* newData = new T[newCapacity];

            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        data[size++] = value;
    }

    void insert(int index, T value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Invalid index");
        }

        if (size >= capacity) {
            int newCapacity = capacity > 0 ? capacity * 2 : 4;
            T* newData = new T[newCapacity];

            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        size++;
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Invalid index");
        }

        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }

        size--;
    }

    void swapElements(int index, T value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Invalid index");
        }

        data[index] = value;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    void print() const {
        std::cout << "[";
        for (int i = 0; i < size; ++i) {
            std::cout << data[i];
            if (i < size - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";
    }

 private:
    T* data;
    int size;
    int capacity;
};
