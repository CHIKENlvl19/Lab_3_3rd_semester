// Copyright message
#pragma once
#include <iostream>
#include <utility>
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
    DL_list()
    : head(nullptr), tail(nullptr), size(0) {}

    DL_list(const DL_list& other)
        : head(nullptr), tail(nullptr), size(0) {
        if (!other.head) {
            return;
        }

        Node* newHead = nullptr;
        Node* newTail = nullptr;

        // копия через локальные переменные (strong exception safety)
        newHead = new Node(other.head->value, nullptr, nullptr);
        Node* curNew = newHead;
        Node* curOther = other.head->next;

        try {
            while (curOther) {
                Node* n = new Node(curOther->value, nullptr, curNew);
                curNew->next = n;
                curNew = n;
                curOther = curOther->next;
            }
            newTail = curNew;
        } catch (...) {
            // удаляем все созданные узлы до выброса
            Node* tmp = newHead;
            while (tmp) {
                Node* next = tmp->next;
                delete tmp;
                tmp = next;
            }
            throw;
        }

        head = newHead;
        tail = newTail;
        size = other.size;
    }

    DL_list<T>& operator=(const DL_list& other) {
        if (this == &other) {
            return *this;
        }

        DL_list<T> tmp(other);  // strong exception safety
        swap(tmp);
        return *this;
    }

    ~DL_list() {
        clear();
    }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head = tail = nullptr;
        size = 0;
    }

    bool empty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void addHead(const T& value) {
        Node* newNode = new Node(value, head, nullptr);
        if (head) {
            head->previous = newNode;
        }
        else tail = newNode;
        head = newNode;
        size++;
    }

    void addTail(const T& value) {
        Node* newNode = new Node(value, nullptr, tail);
        if (tail) {
            tail->next = newNode;
        }
        else head = newNode;
        tail = newNode;
        size++;
    }

    void addAfter(int index, const T& value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range in addAfter");
        }

        if (index == size) {
            addTail(value);
            return;
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

    void addBefore(int index, const T& value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range in addBefore");
        }
        if (index == 0) {
            addHead(value);
            return;
        }
        if (index == size) {
            addTail(value);
            return;
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

    void removeByValue(const T& value) {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            if (cur->value == value) {
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
                --size;
            }
            cur = next;
        }
    }

    int searchByValue(const T& value) const {
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

    void print(std::ostream& os) const {
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

    void printBackwards(std::ostream& os) const {
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

 private:
    void swap(DL_list& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(size, other.size);
    }
};
