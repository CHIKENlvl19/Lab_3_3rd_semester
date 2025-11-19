// Copyright message
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace std;

template <typename T>
struct NodeDL {
    T value;
    NodeDL* next;
    NodeDL* previous;

    explicit NodeDL(T v, NodeDL* n = nullptr, NodeDL* p = nullptr)
        : value(v), next(n), previous(p) {}
};


template <typename T>
class DL_list {
 public:
    NodeDL<T>* head;
    NodeDL<T>* tail;
    int size;

    DL_list() : head(nullptr), tail(nullptr), size(0) {}

    DL_list(const DL_list& other) : head(nullptr), tail(nullptr), size(0) {
        NodeDL<T>* current = other.head;
        while (current) {
            addTail(current->value);
            current = current->next;
        }
    }

    DL_list& operator=(const DL_list& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        NodeDL<T>* current = other.head;
        while (current) {
            addTail(current->value);
            current = current->next;
        }

        return *this;
    }

    ~DL_list() { clear(); }

    void clear() {
        NodeDL<T>* current = head;
        while (current) {
            NodeDL<T>* tmp = current;
            current = current->next;
            delete tmp;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void addHead(const T& value) {
        NodeDL<T>* newNode = new NodeDL<T>(value, head, nullptr);
        if (head) {
            head->previous = newNode;
        } else {
            tail = newNode;
        }

        head = newNode;
        size++;
    }

    void addTail(const T& value) {
        NodeDL<T>* newNode = new NodeDL<T>(value, nullptr, tail);
        if (tail) {
            tail->next = newNode;
        } else {
            head = newNode;
        }

        tail = newNode;
        size++;
    }

    void addAfter(int index, const T& value) {
        if (index < 0 || index >= size) {
            cerr << "Error: index out of range in addAfter\n";
            return;
        }

        NodeDL<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        NodeDL<T>* newNode = new NodeDL<T>(value, current->next, current);
        if (current->next) {
            current->next->previous = newNode;
        } else {
            tail = newNode;
        }

        current->next = newNode;
        size++;
    }

    void addBefore(int index, const T& value) {
        if (index < 0 || index >= size) {
            cerr << "Error: index out of range in addBefore\n";
            return;
        }

        NodeDL<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        NodeDL<T>* newNode = new NodeDL<T>(value, current, current->previous);
        if (current->previous) {
            current->previous->next = newNode;
        } else {
            head = newNode;
        }

        current->previous = newNode;
        size++;
    }

    void removeByValue(const T& value) {
        NodeDL<T>* current = head;
        while (current && current->value != value) {
            current = current->next;
        }
        if (!current) {
            cerr << "Value not found in removeByValue\n";
            return;
        }

        if (current->previous) {
            current->previous->next = current->next;
        } else {
            head = current->next;
        }

        if (current->next) {
            current->next->previous = current->previous;
        } else {
            tail = current->previous;
        }

        delete current;
        size--;
    }

    int searchByValue(const T& value) {
        NodeDL<T>* current = head;
        int index = 0;
        while (current) {
            if (current->value == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }

    void print(ostream& os = cout) {
        NodeDL<T>* current = head;

        os << "[";
        while (current) {
            os << current->value;
            if (current->next) {
                os << " <-> ";
            }
            current = current->next;
        }
        os << "]" << endl;
    }

    void backwardsPrint() {
        NodeDL<T>* current = tail;

        cout << "[";
        while (current) {
            cout << current->value;
            if (current->previous) {
                cout << " <-> ";
            }
            current = current->previous;
        }
        cout << "]" << endl;
    }
};
