// Copyright message
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

const int MAX_STACK_SIZE = 1000;

template <typename T>
class Stack {
 public:
    Stack() : head(nullptr), size(0) {}

    Stack(const Stack& other) : head(nullptr), size(0) {
        vector<string> buf;
        NodeStack* cur = other.head;
        while (cur) {
            buf.push_back(cur->value);
            cur = cur->next;
        }

        for (int i = static_cast<int>(buf.size()) - 1; i >= 0; --i) {
            NodeStack* n = new NodeStack(buf[i]);
            head = n;
            size++;
        }
    }

    Stack& operator=(const Stack& other) {
        if (this == &other) {
            return *this;
        }
        clean();
        vector<T> buf;
        NodeStack* cur = other.head;

        while (cur) {
            buf.push_back(cur->value);
            cur = cur->next;
        }

        for (int i = static_cast<int>(buf.size()) - 1; i >= 0; --i) {
            NodeStack* n = new NodeStack(buf[i], head);
            head = n;
            size++;
        }
        return *this;
    }

    void clean() {
        while (head) {
            NodeStack* tmp = head;
            head = head->next;
            delete tmp;
        }
        size = 0;
    }

    ~Stack() {
        clean();
    }

    void push(const T& value) {
        if (size >= MAX_STACK_SIZE) {
            throw overflow_error("Error: stack is full!");
        }

        NodeStack* newNodeStack = new NodeStack(value, head);
        head = newNodeStack;
        size++;
    }

    void pop() {
        if (size == 0) {
            throw underflow_error("Error: stack is empty! Nothing to delete.");
        }

        NodeStack* toDelete = head;
        head = head->next;
        delete toDelete;
        size--;
    }

    T peek() const {
        if (size == 0) {
            throw underflow_error("Stack is empty!");
        }
        return head->value;
    }

    void print() {
        if (size == 0) {
            cout << "Stack is empty!" << endl;
            return;
        }

        NodeStack* current = head;
            cout << "nullptr";
            while (current != nullptr) {
                cout << " <- " << current->value;
                current = current->next;
            }
            cout << endl;
        }

    int getSize() const {
        return size;
    }

 private:
    struct NodeStack {
        T value;
        NodeStack* next;

         explicit NodeStack(T v, NodeStack* ns = nullptr)
            : value(v), next(ns) {}
    };

    NodeStack* head;
    int size;
};
