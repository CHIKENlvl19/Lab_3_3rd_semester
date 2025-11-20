// Copyright message
#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

const int MAX_STACK_SIZE = 1000;

template <typename T>
class Stack {
 public:
    Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    ~Stack();

    void push(const T& value);
    void pop();
    T peek() const;
    void print() const;
    int getSize() const;

 private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& v, Node* ns = nullptr)
            : value(v), next(ns) {}
    };

    void clean();

    Node* head;
    int size;
};

template <typename T>
Stack<T>::Stack() : head(nullptr), size(0) {}

template <typename T>
Stack<T>::Stack(const Stack& other) : head(nullptr), size(0) {
    if (!other.head) return;

    std::vector<T> buf;
    Node* cur = other.head;
    while (cur) {
        buf.push_back(cur->value);
        cur = cur->next;
    }

    for (int i = static_cast<int>(buf.size()) - 1; i >= 0; --i) {
        head = new Node(buf[i], head);
        size++;
    }
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this == &other) return *this;

    clean();

    std::vector<T> buf;
    Node* cur = other.head;
    while (cur) {
        buf.push_back(cur->value);
        cur = cur->next;
    }

    for (int i = static_cast<int>(buf.size()) - 1; i >= 0; --i) {
        head = new Node(buf[i], head);
        size++;
    }

    return *this;
}

template <typename T>
void Stack<T>::clean() {
    while (head) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
    size = 0;
}

template <typename T>
Stack<T>::~Stack() {
    clean();
}

template <typename T>
void Stack<T>::push(const T& value) {
    if (size >= MAX_STACK_SIZE)
        throw std::overflow_error("Error: stack is full!");

    head = new Node(value, head);
    size++;
}

template <typename T>
void Stack<T>::pop() {
    if (size == 0)
        throw std::underflow_error("Error: stack is empty!");

    Node* toDelete = head;
    head = head->next;
    delete toDelete;
    size--;
}

template <typename T>
T Stack<T>::peek() const {
    if (size == 0)
        throw std::underflow_error("Stack is empty!");

    return head->value;
}

template <typename T>
void Stack<T>::print() const {
    if (size == 0) {
        std::cout << "Stack is empty!\n";
        return;
    }

    Node* current = head;
    std::cout << "nullptr";
    while (current) {
        std::cout << " <- " << current->value;
        current = current->next;
    }
    std::cout << "\n";
}

template <typename T>
int Stack<T>::getSize() const {
    return size;
}
