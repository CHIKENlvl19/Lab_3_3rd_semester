// Copyright message
#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class DL_list {
 private:
    struct Node {
        T value;
        Node* next;
        Node* previous;
        explicit Node(const T& v, Node* n = nullptr, Node* p = nullptr)
            : value(v), next(n), previous(p) {}
    };

    Node* head;
    Node* tail;
    int size;

 public:
    DL_list();
    DL_list(const DL_list& other);
    DL_list& operator=(const DL_list& other);
    ~DL_list();

    void clear();
    bool empty() const;
    int getSize() const;

    void addHead(const T& value);
    void addTail(const T& value);
    void addAfter(int index, const T& value);
    void addBefore(int index, const T& value);

    void removeByValue(const T& value);
    int searchByValue(const T& value) const;

    void print(std::ostream& os = std::cout) const;
    void printBackwards(std::ostream& os = std::cout) const;
};


template <typename T>
DL_list<T>::DL_list()
    : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
DL_list<T>::DL_list(const DL_list& other)
    : head(nullptr), tail(nullptr), size(0) {
    Node* cur = other.head;
    while (cur) {
        addTail(cur->value);
        cur = cur->next;
    }
}

template <typename T>
DL_list<T>& DL_list<T>::operator=
    (const DL_list& other) {
    if (this == &other) {
        return *this;
    }

    clear();
    Node* cur = other.head;
    while (cur) {
        addTail(cur->value);
        cur = cur->next;
    }
    return *this;
}

template <typename T>
DL_list<T>::~DL_list() {
    clear();
}

template <typename T>
void DL_list<T>::clear() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = tail = nullptr;
    size = 0;
}

template <typename T>
bool DL_list<T>::empty() const {
    return size == 0;
}

template <typename T>
int DL_list<T>::getSize() const {
    return size;
}

template <typename T>
void DL_list<T>::addHead(const T& value) {
    Node* newNode = new Node(value, head, nullptr);
    if (head) {
        head->previous = newNode;
    } else {
        tail = newNode;
    }

    head = newNode;
    size++;
}

template <typename T>
void DL_list<T>::addTail(const T& value) {
    Node* newNode = new Node(value, nullptr, tail);
    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode;
    }

    tail = newNode;
    size++;
}

template <typename T>
void DL_list<T>::addAfter(int index, const T& value) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range in addAfter");
    }
    Node* cur = head;
    for (int i = 0; i < index; ++i) {
        cur = cur->next;
    }

    Node* newNode = new Node(value, cur->next, cur);
    if (cur->next) {
        cur->next->previous = newNode;
    } else {
        tail = newNode;
    }

    cur->next = newNode;
    size++;
}

template <typename T>
void DL_list<T>::addBefore(int index, const T& value) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range in addBefore");
    }

    Node* cur = head;
    for (int i = 0; i < index; ++i) {
        cur = cur->next;
    }

    Node* newNode = new Node(value, cur, cur->previous);
    if (cur->previous) {
        cur->previous->next = newNode;
    } else {
        head = newNode;
    }

    cur->previous = newNode;
    size++;
}

template <typename T>
void DL_list<T>::removeByValue(const T& value) {
    Node* cur = head;
    while (cur && cur->value != value) {
        cur = cur->next;
    }

    if (!cur) {
        return;
    }

    if (cur->previous) {
        cur->previous->next = cur->next;
    } else {
        head = cur->next;
    }

    if (cur->next) {
        cur->next->previous = cur->previous;
    } else {
        tail = cur->previous;
    }

    delete cur;
    size--;
}

template <typename T>
int DL_list<T>::searchByValue(const T& value) const {
    Node* cur = head;
    int index = 0;
    while (cur) {
        if (cur->value == value) {
            return index;
        }

        cur = cur->next;
        ++index;
    }
    return -1;
}

template <typename T>
void DL_list<T>::print(std::ostream& os) const {
    Node* cur = head;
    os << "[";
    while (cur) {
        os << cur->value;
        if (cur->next) {
            os << " <-> ";
        }

        cur = cur->next;
    }
    os << "]\n";
}

template <typename T>
void DL_list<T>::printBackwards(std::ostream& os) const {
    Node* cur = tail;
    os << "[";
    while (cur) {
        os << cur->value;
        if (cur->previous) {
            os << " <-> ";
        }

        cur = cur->previous;
    }
    os << "]\n";
}
