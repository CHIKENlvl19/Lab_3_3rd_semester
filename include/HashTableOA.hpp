// Copyright message
#pragma once
#include <iostream>
#include <string>
#include <random>
#include <type_traits>

template <typename Key, typename Value>
class HashTableOA {
 public:
    explicit HashTableOA(int capacity = 10000);
    ~HashTableOA();

    void clean();

    size_t getSize() const;
    size_t getCapacity() const;
    float getLoadFactor() const;

    bool insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    bool isPresent(const Key& key) const;
    Value find(const Key& key) const;

    void print() const;

 private:
    struct Cell {
        Key key;
        Value value;
        bool isOccupied;
        bool isDeleted;

        Cell();
    };

    Cell* table;
    size_t size;
    size_t capacity;
    float loadFactor;

    int a, b;
    int p;

    void init();
    size_t h1(const Key& key) const;
};

//
// ---- Определения методов ----
//

template <typename Key, typename Value>
HashTableOA<Key, Value>::Cell::Cell() : isOccupied(false), isDeleted(false) {}

template <typename Key, typename Value>
HashTableOA<Key, Value>::HashTableOA(int capacity)
    : size(0), capacity(capacity), loadFactor(0.0f), p(1000000007) {
    table = new Cell[capacity];
    init();
}

template <typename Key, typename Value>
HashTableOA<Key, Value>::~HashTableOA() {
    clean();
}

template <typename Key, typename Value>
void HashTableOA<Key, Value>::clean() {
    delete[] table;
    table = nullptr;
    size = 0;
    loadFactor = 0.0f;
}

template <typename Key, typename Value>
size_t HashTableOA<Key, Value>::getSize() const {
    return size;
}

template <typename Key, typename Value>
size_t HashTableOA<Key, Value>::getCapacity() const {
    return capacity;
}

template <typename Key, typename Value>
float HashTableOA<Key, Value>::getLoadFactor() const {
    return static_cast<float>(size) / capacity;
}

template <typename Key, typename Value>
void HashTableOA<Key, Value>::init() {
    std::mt19937 gen(1337);
    std::uniform_int_distribution<int> dist(1, 1000);
    a = dist(gen);
    b = dist(gen);
}

template <typename Key, typename Value>
size_t HashTableOA<Key, Value>::h1(const Key& key) const {
    uint64_t keyValue = 0;

    if constexpr (std::is_integral_v<Key> || std::is_floating_point_v<Key>) {
        keyValue = static_cast<uint64_t>(key);
    } else {
        for (char c : key) {
            keyValue = keyValue * 131 + static_cast<unsigned char>(c);
        }
    }

    return static_cast<size_t>(((a * keyValue + b) % p) % capacity);
}

template <typename Key, typename Value>
bool HashTableOA<Key, Value>::insert(const Key& key, const Value& value) {
    size_t index;
    for (size_t i = 0; i < capacity; i++) {
        index = (h1(key) + i) % capacity;

        if (!table[index].isOccupied || table[index].isDeleted) {
            table[index].key = key;
            table[index].value = value;
            table[index].isOccupied = true;
            table[index].isDeleted = false;
            size++;
            loadFactor = getLoadFactor();
            return true;
        }

        if (table[index].isOccupied && !table[index].isDeleted
                && table[index].key == key) {
            table[index].value = value;
            return true;
        }
    }

    std::cerr << "Error: table is full!" << std::endl;
    return false;
}

template <typename Key, typename Value>
bool HashTableOA<Key, Value>::isPresent(const Key& key) const {
    size_t index;
    for (size_t i = 0; i < capacity; i++) {
        index = (h1(key) + i) % capacity;
        if (!table[index].isOccupied && !table[index].isDeleted) return false;
        if (table[index].isOccupied && table[index].key == key) return true;
    }
    return false;
}

template <typename Key, typename Value>
Value HashTableOA<Key, Value>::find(const Key& key) const {
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
    return Value();
}

template <typename Key, typename Value>
bool HashTableOA<Key, Value>::remove(const Key& key) {
    size_t index;
    for (size_t i = 0; i < capacity; i++) {
        index = (h1(key) + i) % capacity;
        if (!table[index].isOccupied && !table[index].isDeleted) return false;
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

template <typename Key, typename Value>
void HashTableOA<Key, Value>::print() const {
    for (size_t i = 0; i < capacity; i++) {
        std::cout << "[" << i << "]";
        if (table[i].isOccupied) {
            std::cout << " {" << table[i].key << ": " << table[i].value << "}";
        } else if (table[i].isDeleted) {
            std::cout << "(deleted)";
        }
        std::cout << std::endl;
    }
}
