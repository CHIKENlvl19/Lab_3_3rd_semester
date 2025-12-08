// Copyright message
#pragma once

#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdexcept>
#include <utility>
#include "../include/DL_List.hpp"


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
            //  std::cerr << "Error, key " << key
            //    << " is already in the table!" << std::endl;
        }
    }


    void remove(const Key& key) {
        if (!isPresent(key)) {
            //  std::cerr << "Error, key " << key
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

    void print() const {
        for (int i = 0; i < capacity; i++) {
            std::cout << "[" << i << "]: ";

            auto* node = buckets[i].values.getHead();
            std::cout << "[";
            while (node) {
                std::cout << "(" << node->value.key << ":"
                    << node->value.value << ")";
                if (node->next) std::cout << " <-> ";
                node = node->next;
            }
            std::cout << "]\n";
        }
    }

    // текстовый формат
    void saveText(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }

        // сохраняем метаданные
        file << size << " " << capacity << "\n";
        file << a.get_str() << "\n";
        file << b.get_str() << "\n";
        file << p.get_str() << "\n";

        // сохраняем все пары key-value
        for (int i = 0; i < capacity; i++) {
            auto* node = buckets[i].values.getHead();
            while (node) {
                file << node->value.key << " " << node->value.value << "\n";
                node = node->next;
            }
        }
        file.close();
    }

    void loadText(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }

        // очищаем старые данные
        if (buckets) {
            delete[] buckets;
        }

        // читаем метаданные
        size_t newSize;
        int newCapacity;
        file >> newSize >> newCapacity;
        file.ignore();  // skip newline

        std::string aStr, bStr, pStr;
        std::getline(file, aStr);
        std::getline(file, bStr);
        std::getline(file, pStr);

        a = mpz_class(aStr);
        b = mpz_class(bStr);
        p = mpz_class(pStr);

        capacity = newCapacity;
        size = 0;
        loadFactor = 0.0f;
        buckets = new Bucket<Key, Value>[capacity];

        // читаем и вставляем пары
        for (size_t i = 0; i < newSize; ++i) {
            Key key;
            Value value;
            file >> key >> value;
            insert(key, value);
        }
        file.close();
    }

    // бинарный формат
    void saveBinary(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing");
        }

        // сохраняем size и capacity
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));

        // сохраняем параметры хеш-функции как строки
        std::string aStr = a.get_str();
        std::string bStr = b.get_str();
        std::string pStr = p.get_str();

        size_t aLen = aStr.size();
        size_t bLen = bStr.size();
        size_t pLen = pStr.size();

        file.write(reinterpret_cast<const char*>(&aLen), sizeof(aLen));
        file.write(aStr.c_str(), aLen);

        file.write(reinterpret_cast<const char*>(&bLen), sizeof(bLen));
        file.write(bStr.c_str(), bLen);

        file.write(reinterpret_cast<const char*>(&pLen), sizeof(pLen));
        file.write(pStr.c_str(), pLen);

        // сохраняем все пары
        for (int i = 0; i < capacity; i++) {
            auto* node = buckets[i].values.getHead();
            while (node) {
                file.write(reinterpret_cast<const char*>(&node->value.key)
                    , sizeof(Key));
                file.write(reinterpret_cast<const char*>(&node->value.value)
                    , sizeof(Value));
                node = node->next;
            }
        }
        file.close();
    }

    void loadBinary(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading");
        }

        // очищаем старые данные
        if (buckets) {
            delete[] buckets;
        }

        // метаданные
        size_t newSize;
        int newCapacity;
        file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
        file.read(reinterpret_cast<char*>(&newCapacity), sizeof(newCapacity));

        // параметры хеш-функции
        size_t aLen, bLen, pLen;

        file.read(reinterpret_cast<char*>(&aLen), sizeof(aLen));
        char* aBuffer = new char[aLen + 1];
        file.read(aBuffer, aLen);
        aBuffer[aLen] = '\0';

        file.read(reinterpret_cast<char*>(&bLen), sizeof(bLen));
        char* bBuffer = new char[bLen + 1];
        file.read(bBuffer, bLen);
        bBuffer[bLen] = '\0';

        file.read(reinterpret_cast<char*>(&pLen), sizeof(pLen));
        char* pBuffer = new char[pLen + 1];
        file.read(pBuffer, pLen);
        pBuffer[pLen] = '\0';

        a = mpz_class(aBuffer);
        b = mpz_class(bBuffer);
        p = mpz_class(pBuffer);

        delete[] aBuffer;
        delete[] bBuffer;
        delete[] pBuffer;

        capacity = newCapacity;
        size = 0;
        loadFactor = 0.0f;
        buckets = new Bucket<Key, Value>[capacity];

        // Читаем и вставляем пары
        for (size_t i = 0; i < newSize; ++i) {
            Key key;
            Value value;
            file.read(reinterpret_cast<char*>(&key), sizeof(Key));
            file.read(reinterpret_cast<char*>(&value), sizeof(Value));
            insert(key, value);
        }
        file.close();
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
