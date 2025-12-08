// Copyright message
#pragma once
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

template <typename T>
class SinglyLinkedList {
 public:
    SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    SinglyLinkedList(const SinglyLinkedList& other)
        : head(nullptr), tail(nullptr), size(0) {
        Node* cur = other.head;
        while (cur) {
            pushBack(cur->value);
            cur = cur->next;
        }
    }

    SinglyLinkedList<T>& operator=
        (const SinglyLinkedList& other) {
        if (this == &other)
            return *this;

        clear();
        Node* cur = other.head;
        while (cur) {
            pushBack(cur->value);
            cur = cur->next;
        }
        return *this;
    }

    ~SinglyLinkedList() {
        clear();
    }

    void pushFront(const T& value) {
        head = new Node(value, head);
        if (size == 0)
            tail = head;
        size++;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value);

        if (!head) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }

        size++;
    }

    void popFront() {
        if (!head)
            throw std::underflow_error("List is empty");

        Node* tmp = head;
        head = head->next;

        if (!head) {
            tail = nullptr;
        }  // список стал пустым

        delete tmp;
        size--;
    }

    void popBack() {
        if (!head)
            throw std::underflow_error("List is empty");

        // один элемент
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
            size = 0;
            return;
        }

        // ищем предпоследний
        Node* cur = head;
        while (cur->next != tail)
            cur = cur->next;

        delete tail;
        tail = cur;
        tail->next = nullptr;
        size--;
    }

    T& front() const {
        if (!head)
            throw std::underflow_error("List is empty");
        return head->value;
    }

    int getSize() const {
        return size;
    }

    bool empty() const {
        return size == 0;
    }

    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void print() const {
        Node* cur = head;
        std::cout << "Head -> ";
        while (cur) {
            std::cout << cur->value << " -> ";
            cur = cur->next;
        }
        std::cout << "nullptr\n";
    }

    // текстовый формат
    void saveText(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }

        file << size << "\n";
        Node* cur = head;
        while (cur) {
            file << cur->value;
            if (cur->next) file << " ";
            cur = cur->next;
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
            pushBack(value);
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

        Node* cur = head;
        while (cur) {
            file.write(reinterpret_cast<const char*>(&cur->value), sizeof(T));
            cur = cur->next;
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
            pushBack(value);
        }
        file.close();
    }


 private:
    struct Node {
        T value;
        Node* next;
        explicit Node(const T& v, Node* n = nullptr)
            : value(v), next(n) {}
    };

    Node* head;
    Node* tail;
    int size;
};
