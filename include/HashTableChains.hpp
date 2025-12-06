// Copyright message
#pragma once

#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdexcept>
#include <utility>
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

// Хелпер-класс для безопасного управления gmp_randstate_t
class GmpStateWrapper {
 public:
    // 1. Конструктор: Инициализирует ресурс
    GmpStateWrapper() {
        gmp_randinit_default(state_);
    }

    // 2. Деструктор: Освобождает ресурс
    ~GmpStateWrapper() {
        gmp_randclear(state_);
    }

    // 3. Запрещаем копирование (ресурс уникальный)
    GmpStateWrapper(const GmpStateWrapper&) = delete;
    GmpStateWrapper& operator=(const GmpStateWrapper&) = delete;

    // 4. Геттер для доступа к gmp_randstate_t извне
    gmp_randstate_t& get_state() noexcept { return state_; }
    const gmp_randstate_t& get_state() const noexcept { return state_; }

    // 5. Вспомогательный метод обмена (для использования в HashTable::swap)
    void swap(GmpStateWrapper& other) noexcept {
        // gmp_randstate_t — это C-структура/массив, который можно безопасно
        // обменять побитно, чтобы обменять указатель на ресурс.
        std::swap(state_, other.state_);
    }

 private:
    gmp_randstate_t state_;
};

template <typename Key, typename Value>
class HashTable {
 public:
    HashTable()
        : buckets(nullptr), loadFactor(0.0f), size(0), capacity(5) {
        init();
    }

    explicit HashTable(int cap)
        : buckets(nullptr), loadFactor(0.0f), size(0), capacity(cap) {
        if (capacity <= 0) capacity = 5;
        init();
    }

    HashTable(const HashTable& other)
        : buckets(nullptr),
        loadFactor(other.loadFactor),
        size(other.size),
        capacity(other.capacity),
        a(other.a),
        b(other.b),
        p(other.p)  {
        buckets = new Bucket<Key, Value>[capacity];

        for (int i = 0; i < capacity; i++) {
            auto* node = other.buckets[i].values.getHead();   // или getHead()
            while (node) {
                buckets[i].values.addTail(node->value);
                node = node->next;
            }
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            HashTable<Key, Value> tmp(other);  // Глубокая копия
            swap(tmp);
        }

        return *this;
    }

    ~HashTable() {
        clean();
        // gmp_randclear(state);
    }


    int hashing(const Key& key) const {
        mpz_class key_mpz = key;
        mpz_class hash = ((a * key_mpz + b) % p) % capacity;
        return static_cast<int>(hash.get_ui());
    }


    void insert(const Key& key, const Value& value) {
        int index = hashing(key);
        if (searchPairByKey(buckets[index].values, key) == -1) {
            buckets[index].values.addTail(Pair<Key, Value>(key, value));
            size++;
            loadFactor = static_cast<float>(size) / capacity;
        } else {
            //std::cerr << "Error, key " << key
            //    << " is already in the table!" << std::endl;
        }
    }


    void remove(const Key& key) {
        if (!isPresent(key)) {
            //std::cerr << "Error, key " << key
            //    << " is not present in table!" << std::endl;
            return;
        }

        int index = hashing(key);

        Value val = find(key);

        buckets[index].values.removeByValue(Pair<Key, Value>(key, val));

        size--;
        loadFactor = static_cast<float>(size) / capacity;
    }


    // и для большей устойчивости можно чуть подстраховать isPresent:
    bool isPresent(const Key& key) {
        if (!buckets) return false;  // защитная проверка — на всякий случай
        int index = hashing(key);
        return searchPairByKey(buckets[index].values, key) != -1;
    }


    Value find(const Key& key) {
        int index = hashing(key);
        auto* cur = buckets[index].values.getHead();

        while (cur) {
            if (cur->value.key == key) {
                return cur->value.value;
            }
            cur = cur->next;
        }
        return Value();
    }


    void clean() {
        if (buckets) {
            delete[] buckets;
            buckets = nullptr;  // ← не выделяем новую память!
        }
        size = 0;
        loadFactor = 0.0f;
    }

    void print() {
        std::ofstream out("hash_table.txt");

        for (int i = 0; i < capacity; i++) {
            std::cout << "[" << i << "]: ";
            buckets[i].values.print();

            out << "[" << i << "]: ";
            buckets[i].values.print(out);
        }
        out.close();
    }

 private:
    Bucket<Key, Value>* buckets;
    float loadFactor;
    size_t size;
    int capacity;

    mpz_class a, b, p;
    // gmp_randstate_t state;
    GmpStateWrapper state_wrapper;

    mpz_class get_random_64() {
        mpz_class r;
        // mpz_urandomb(r.get_mpz_t(), state, 64);
        mpz_urandomb(r.get_mpz_t(), state_wrapper.get_state(), 64);
        return r;
    }

    void init() {
        buckets = new Bucket<Key, Value>[capacity];
        // gmp_randinit_default(state);
        std::random_device rd;
        mpz_class seed = rd();
        // gmp_randseed(state, seed.get_mpz_t());
        gmp_randseed(state_wrapper.get_state(), seed.get_mpz_t());  // Используем get_state()

        a = get_random_64();
        b = get_random_64();
        // p = generate_safe_prime(state, 64);
        p = generate_safe_prime(state_wrapper, 64);  // Используем get_state()
    }

    int searchPairByKey
        (DL_list<Pair<Key, Value>>& list, const Key& key) {
        auto* cur = list.getHead();
        int idx = 0;
        while (cur) {
            if (cur->value.key == key) return idx;
            cur = cur->next;
            idx++;
        }
        return -1;
    }


    /* mpz_class generate_safe_prime
        (gmp_randstate_t state, int bits) {
        mpz_class prime;
        mpz_urandomb(prime.get_mpz_t(), state, bits);
        mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());
        return prime;
    }*/

    mpz_class generate_safe_prime
        (GmpStateWrapper& state_wrapper, int bits) {
        mpz_class prime;
        mpz_urandomb(prime.get_mpz_t(), state_wrapper.get_state(), bits);
        mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());
        return prime;
    }

    void swap(HashTable& other) noexcept {
        std::swap(buckets, other.buckets);
        std::swap(loadFactor, other.loadFactor);
        std::swap(size, other.size);
        std::swap(capacity, other.capacity);

        std::swap(a, other.a);
        std::swap(b, other.b);
        std::swap(p, other.p);

        state_wrapper.swap(other.state_wrapper);
    }
};
