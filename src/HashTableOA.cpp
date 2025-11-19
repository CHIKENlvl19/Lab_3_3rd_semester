// Copyright message
#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

template <typename Key, typename Value>
class HashTableOA {
 public:
    explicit HashTableOA(int capacity = 10000)
        : size(0), capacity(capacity), loadFactor(0.0f) {
        table = new Cell[capacity];
        init();
    }

    void clean() {
        delete[] table;
        table = nullptr;
        size = 0;
        loadFactor = 0.0f;
    }

    ~HashTableOA() {
        clean();
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    float getLoadFactor() const {
        return static_cast<float>(size) / capacity;
    }

    // h(k) = ((a*k + b) mod p) mod m
    size_t h1(const Key& key) const {
        uint64_t keyValue;

        if constexpr (is_integral_v<Key> || is_floating_point_v<Key>) {
            keyValue = static_cast<uint64_t>(key);
        } else {
            uint64_t hash = 0;
            for (char c : key) {
                hash = hash * 131 + static_cast<unsigned char>(c);
            }

            keyValue = hash;
        }

        return static_cast<size_t>(((a * keyValue + b) % p) % capacity);
    }

    // линейная пробация: (h1 + i) % m
    bool insert(const Key& key, const Value& value) {
        size_t index;

        for (size_t i = 0; i < capacity; i++) {
            index = (h1(key) + i) % capacity;

            // вставка в свободную или удалённую ячейку
            if (!table[index].isOccupied || table[index].isDeleted) {
                table[index].key = key;
                table[index].value = value;
                table[index].isOccupied = true;
                table[index].isDeleted = false;
                size++;
                loadFactor = getLoadFactor();
                return true;
            }

            // обновление значения, если ключ уже существует
            if (table[index].isOccupied && !table[index].isDeleted
                && table[index].key == key) {
                table[index].value = value;
                return true;
            }
        }

        cerr << "Error: table is full!" << endl;
        return false;
    }

    bool isPresent(const Key& key) const {
        size_t index;

        for (size_t i = 0; i < capacity; i++) {
            index = (h1(key) + i) % capacity;

            if (!table[index].isOccupied && !table[index].isDeleted) {
                return false;
            }

            if (table[index].isOccupied && table[index].key == key) {
                return true;
            }
        }

        return false;
    }

    Value find(const Key& key) const {
        size_t index;

        for (size_t i = 0; i < capacity; i++) {
            index = (h1(key) + i) % capacity;

            if (!table[index].isOccupied && !table[index].isDeleted) {
                break;
            }

            if (table[index].isOccupied && table[index].key == key) {
                return table[index].value;
            }
        }

        return Value();  // значение по умолчанию
    }

    bool remove(const Key& key) {
        size_t index;

        for (size_t i = 0; i < capacity; i++) {
            index = (h1(key) + i) % capacity;

            if (!table[index].isOccupied && !table[index].isDeleted) {
                return false;
            }

            if (table[index].isOccupied && table[index].key == key) {
                table[index].isDeleted = true;
                table[index].isOccupied = false;
                size--;
                loadFactor = getLoadFactor();
                return true;
            }
        }

        return false;
    }

    void print() const {
        for (size_t i = 0; i < capacity; i++) {
            cout << "[" << i << "]";
            if (table[i].isOccupied) {
                cout << " {" << table[i].key << ": " << table[i].value << "}";
            } else if (table[i].isDeleted) {
                cout << "(deleted)";
            }
            cout << endl;
        }
    }

 private:
    struct Cell {
        Key key;
        Value value;
        bool isOccupied;
        bool isDeleted;

        Cell() : isOccupied(false), isDeleted(false) {}
    };

    Cell* table;
    size_t size;
    size_t capacity;
    float loadFactor;

    int a, b;
    int p = 1000000007;

    void init() {
        mt19937 gen(1337);
        uniform_int_distribution<int> dist(1, 1000);

        a = dist(gen);
        b = dist(gen);
    }
};
