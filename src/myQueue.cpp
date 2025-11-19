// Copyright message
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class myQueue {
 private:
    T* data;
    int head;
    int tail;
    int size;
    int capacity;

    explicit myQueue(int initialCapacity)
    : data(nullptr), head(0), tail(0), size(0), capacity(0) {
        if (initialCapacity <= 0) {
            initialCapacity = 4;
        }
        data = new T[initialCapacity];
        capacity = initialCapacity;
    }

    ~myQueue() {
        clean();
    }

 public:
    void clean() {
        delete[] data;
        data = nullptr;
        head = 0;
        tail = 0;
        size = 0;
        capacity = 0;
    }

    void loadFromFile(myQueue& queue, const T& filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            queue.clean();
            queue.capacity = 4;
            queue.data = new T[queue.capacity];
            return;
        }

        vector<T> tmp;
        T line;
        while getline(in, line)) {
            tmp.push_back(line);
        }
        in.close();

        queue.clean();
        int cap = 4;
        while (cap < static_cast<int>(tmp.size())) {
            cap *= 2;
        }

        queue.capacity = cap;
        queue.data = new T[queue.capacity];
        queue.size = static_cast<int>(tmp.size());
        queue.head = 0;
        queue.tail = queue.size;

        for (int i = 0; i < queue.size; ++i) {
            queue.data[i] = tmp[i];
        }
    }

    void saveToFile(const myQueue& queue, const T& filename) {
        ofstream out(filename, ios::trunc);
        if (!out.is_open()) {
            throw runtime_error("Cannot open queue file for writing: " + filename);
        }
        for (int i = 0; i < queue.size; ++i) {
            out << queue.data[(queue.head + i) % queue.capacity] << "\n";
        }

        out.close();
    }

    void push(myQueue& queue, const T& value, const T& filename) {
        loadFromFile(queue, filename);

        if (queue.size == queue.capacity) {
            int newCapacity = queue.capacity * 2;
            T* newData = new T[newCapacity];

            for (int i = 0; i < queue.size; ++i) {
                newData[i] = queue.data[(queue.head + i) % queue.capacity];
            }

            delete[] queue.data;
            queue.data = newData;
            queue.capacity = newCapacity;
            queue.head = 0;
            queue.tail = queue.size;
        }

        queue.data[queue.tail] = value;
        queue.tail = (queue.tail + 1) % queue.capacity;
        queue.size++;

        saveToFile(queue, filename);
    }

    void pop(myQueue& queue, const T& filename) {
        loadFromFile(queue, filename);

        if (queue.size == 0) {
            cerr << "Error: queue is empty, nothing to delete!" << endl;
            return;
        }

        queue.head = (queue.head + 1) % queue.capacity;
        queue.size--;

        saveToFile(queue, filename);
    }

    void print(myQueue& queue) {
        if (queue.size == 0) {
            cout << "Queue is empty!" << endl;
            return;
        }

        for (int i = 0; i < queue.size; i++) {
            cout << queue.data[(queue.head + i) % queue.capacity] << " ";
        }
        cout << endl;
    }

    int size(const myQueue& queue) {
        return queue.size;
    }
};
