#include <iostream>
using namespace std;

template <typename T>
class myArray {
 public:
        explicit myArray(int initialCapacity)
        : data(nullptr), size(0), capacity(0) {
        if (initialCapacity <= 0) {
            initialCapacity = 4;
        }
        data = new T[initialCapacity];
        capacity = initialCapacity;
    }

    myArray(const myArray& other)
        : data(nullptr), size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    myArray& operator=(const myArray& other) {
        if (this == &other) {
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
            throw out_of_range("Error: inserting failed due to invalid index.");
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

    T& getElement(int index) {
        if (index < 0) {
            throw out_of_range("Error: getting an element failed due to invalid index.");
        }
        return data[index];
    }

    void remove(int index) {

        if (index < 0 || index >= size) {
            throw out_of_range("Error: deleting an element failed due to invalid index.");
        }

        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        
        size--;
    }

    void swapElements(int index, T value) {

        if (index < 0 || index >= size) {
            throw out_of_range("Error: swapping an element failed due to invalid index.");
        }

        data[index] = value;
    }

    int getSize() {
        return size;
    }

    void print() {
        cout << "[";
        for (int i = 0; i < size; ++i) {
            cout << data[i];
            if (i < size - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }

 private:
    T* data;
    int size;
    int capacity;
};
