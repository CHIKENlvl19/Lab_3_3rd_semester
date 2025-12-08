// Copyright message
#pragma once

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

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

    // текстовый формат
    void saveText(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }
        file << size << " " << capacity << "\n";
        for (int i = 0; i < size; ++i) {
            file << data[i];
            if (i < size - 1) file << " ";
        }
        file.close();
    }

    void loadText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }
        int newSize, newCapacity;
        file >> newSize >> newCapacity;

        delete[] data;
        capacity = newCapacity;
        size = newSize;
        data = new T[capacity];

        for (int i = 0; i < size; ++i) {
            file >> data[i];
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
        file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
        for (int i = 0; i < size; ++i) {
            file.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
        }
        file.close();
    }

    void loadBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }
        int newSize, newCapacity;
        file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
        file.read(reinterpret_cast<char*>(&newCapacity), sizeof(newCapacity));

        delete[] data;
        capacity = newCapacity;
        size = newSize;
        data = new T[capacity];

        for (int i = 0; i < size; ++i) {
            file.read(reinterpret_cast<char*>(&data[i]), sizeof(T));
        }
        file.close();
    }

 private:
    T* data;
    int size;
    int capacity;
};
