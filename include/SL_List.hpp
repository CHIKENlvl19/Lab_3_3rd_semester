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
    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    ~SinglyLinkedList();

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront();
    T& front() const;
    int getSize() const;
    bool empty() const;

    void clear();
    void print() const;
};


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head(nullptr), size(0) {}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList& other)
    : head(nullptr), size(0) {
    Node* cur = other.head;
    while (cur) {
        pushBack(cur->value);
        cur = cur->next;
    }
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=
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

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    clear();
}

template <typename T>
void SinglyLinkedList<T>::pushFront(const T& value) {
    head = new Node(value, head);
    size++;
}

template <typename T>
void SinglyLinkedList<T>::pushBack(const T& value) {
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

template <typename T>
void SinglyLinkedList<T>::popFront() {
    if (!head)
        throw std::underflow_error("List is empty");

    Node* tmp = head;
    head = head->next;
    delete tmp;
    size--;
}

template <typename T>
T& SinglyLinkedList<T>::front() const {
    if (!head)
        throw std::underflow_error("List is empty");
    return head->value;
}

template <typename T>
int SinglyLinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool SinglyLinkedList<T>::empty() const {
    return size == 0;
}

template <typename T>
void SinglyLinkedList<T>::clear() {
    while (head) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
    size = 0;
}

template <typename T>
void SinglyLinkedList<T>::print() const {
    Node* cur = head;
    std::cout << "Head -> ";
    while (cur) {
        std::cout << cur->value << " -> ";
        cur = cur->next;
    }
    std::cout << "nullptr\n";
}
