// Copyright message
#pragma once
#include <stdexcept>
#include <iostream>

template <typename T>
class SinglyLinkedList {
 private:
    struct Node {
        T value;
        Node* next;
        explicit Node(const T& v, Node* n = nullptr)
            : value(v), next(n) {}
    };

    Node* head;
    int size;

 public:
    SinglyLinkedList() : head(nullptr), size(0) {}

    SinglyLinkedList(const SinglyLinkedList& other)
        : head(nullptr), size(0) {
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
        size++;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value);

        if (!head) {
            head = n;
        } else {
            Node* cur = head;
            while (cur->next)
                cur = cur->next;
            cur->next = n;
        }
        size++;
    }

    void popFront() {
        if (!head)
            throw std::underflow_error("List is empty");

        Node* tmp = head;
        head = head->next;
        delete tmp;
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
};
