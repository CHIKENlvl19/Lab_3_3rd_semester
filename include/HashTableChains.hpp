// Copyright message
#pragma once

#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdexcept>
#include "../include/DL_List.hpp"  // шаблон двусвязного списка


template <typename Key, typename Value>
struct Pair {
    Key key;
    Value value;

    Pair() {}
    Pair(const Key& k, const Value& v) : key(k), value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const Pair& p) {
        os << "(" << p.key << " : " << p.value << ")";
        return os;
    }

    bool operator==(const Pair& other) const {
        return key == other.key && value == other.value;
    }

    bool operator!=(const Pair& other) const {
        return !(*this == other);
    }
};

template <typename Key, typename Value>
struct Bucket {
    DL_list<Pair<Key, Value>> values;
};

template <typename Key, typename Value>
class HashTable {
 public:
    HashTable();
    explicit HashTable(int cap);
    ~HashTable();

    int hashing(const Key& key) const;

    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    bool isPresent(const Key& key);
    Value find(const Key& key);

    void clean();
    void print();

 private:
    Bucket<Key, Value>* buckets;
    float loadFactor;
    size_t size;
    int capacity;

    mpz_class a, b, p;
    gmp_randstate_t state;

    mpz_class get_random_64();
    void init();
    int searchPairByKey(DL_list<Pair<Key, Value>>& list, const Key& key);

    static mpz_class generate_safe_prime(gmp_randstate_t state, int bits);
};


template <typename Key, typename Value>
mpz_class HashTable<Key, Value>::generate_safe_prime
    (gmp_randstate_t state, int bits) {
    mpz_class prime;
    mpz_urandomb(prime.get_mpz_t(), state, bits);
    mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());
    return prime;
}

template <typename Key, typename Value>
HashTable<Key, Value>::HashTable()
    : buckets(nullptr), loadFactor(0.0f), size(0), capacity(5) {
    init();
}

template <typename Key, typename Value>
HashTable<Key, Value>::HashTable(int cap)
    : buckets(nullptr), loadFactor(0.0f), size(0), capacity(cap) {
    if (capacity <= 0) capacity = 5;
    init();
}

template <typename Key, typename Value>
HashTable<Key, Value>::~HashTable() {
    clean();
    gmp_randclear(state);
}

template <typename Key, typename Value>
mpz_class HashTable<Key, Value>::get_random_64() {
    mpz_class r;
    mpz_urandomb(r.get_mpz_t(), state, 64);
    return r;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::init() {
    buckets = new Bucket<Key, Value>[capacity];
    gmp_randinit_default(state);
    std::random_device rd;
    mpz_class seed = rd();
    gmp_randseed(state, seed.get_mpz_t());

    a = get_random_64();
    b = get_random_64();
    p = generate_safe_prime(state, 64);
}

template <typename Key, typename Value>
int HashTable<Key, Value>::hashing(const Key& key) const {
    mpz_class key_mpz = key;
    mpz_class hash = ((a * key_mpz + b) % p) % capacity;
    return static_cast<int>(hash.get_ui());
}

template <typename Key, typename Value>
int HashTable<Key, Value>::searchPairByKey
    (DL_list<Pair<Key, Value>>& list, const Key& key) {
    auto* cur = list.head;
    int idx = 0;
    while (cur) {
        if (cur->value.key == key) return idx;
        cur = cur->next;
        idx++;
    }
    return -1;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::insert(const Key& key, const Value& value) {
    int index = hashing(key);
    if (searchPairByKey(buckets[index].values, key) == -1) {
        buckets[index].values.addTail(Pair<Key, Value>(key, value));
        size++;
        loadFactor = static_cast<float>(size) / capacity;
    } else {
        std::cerr << "Error, key " << key
             << " is already in the table!" << std::endl;
    }
}

template <typename Key, typename Value>
void HashTable<Key, Value>::remove(const Key& key) {
    int index = hashing(key);
    int found = searchPairByKey(buckets[index].values, key);

    if (found == -1) {
        std::cerr << "Error, key " << key
            << " is not present in table!" << std::endl;
        return;
    }

    buckets[index].values.removeByValue(Pair<Key, Value>(key, Value()));
    size--;
    loadFactor = static_cast<float>(size) / capacity;
}

template <typename Key, typename Value>
bool HashTable<Key, Value>::isPresent(const Key& key) {
    int index = hashing(key);
    return searchPairByKey(buckets[index].values, key) != -1;
}

template <typename Key, typename Value>
Value HashTable<Key, Value>::find(const Key& key) {
    int index = hashing(key);
    auto* cur = buckets[index].values.head;

    while (cur) {
        if (cur->value.key == key) {
            return cur->value.value;
        }
        cur = cur->next;
    }
    return Value();
}

template <typename Key, typename Value>
void HashTable<Key, Value>::clean() {
    if (buckets) {
        delete[] buckets;
        buckets = nullptr;
    }
    size = 0;
    loadFactor = 0.0f;
}

template <typename Key, typename Value>
void HashTable<Key, Value>::print() {
    std::ofstream out("hash_table.txt");

    for (int i = 0; i < capacity; i++) {
        std::cout << "[" << i << "]: ";
        buckets[i].values.print();

        out << "[" << i << "]: ";
        buckets[i].values.print(out);
    }
    out.close();
}
