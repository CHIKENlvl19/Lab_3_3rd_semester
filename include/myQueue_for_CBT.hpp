// Copyright message
#pragma once
#include <stdexcept>

template <typename T>
class CBTQueue {
 public:
    explicit CBTQueue(int initialCapacity = 4)
        : data(new T[initialCapacity]),
          head(0), tail(0), size(0), capacity(initialCapacity) {}

    CBTQueue(const CBTQueue& other)
        : data(new T[other.capacity]),
          head(other.head),
          tail(other.tail),
          size(other.size),
          capacity(other.capacity) {
        for (int i = 0; i < capacity; ++i)
            data[i] = other.data[i];
    }

    ~CBTQueue() {
        delete[] data;
    }

    bool empty() const {
        return size == 0;
    }

    void push(const T& value) {
        if (size == capacity) {
            grow();
        }

        data[tail] = value;
        tail = (tail + 1) % capacity;
        size++;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }

        head = (head + 1) % capacity;
        size--;
    }

    T& front() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }

        return data[head];
    }

    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }

        return data[head];
    }

 private:
    T* data;
    int head, tail;
    int size;
    int capacity;

    void grow() {
        int newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size; ++i) {
            newData[i] = data[(head + i) % capacity];
        }

        delete[] data;
        data = newData;

        head = 0;
        tail = size;
        capacity = newCapacity;
    }
};
