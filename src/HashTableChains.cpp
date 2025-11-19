// Copyright message
#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "DL_List.h"

using namespace std;

mpz_class generate_safe_prime(gmp_randstate_t state, int bits) {
    mpz_class prime;
    mpz_urandomb(prime.get_mpz_t(), state, bits);
    mpz_nextprime(prime.get_mpz_t(), prime.get_mpz_t());
    return prime;
}

template <typename Key, typename Value>
struct Pair {
    Key key;
    Value value;

    Pair() {}
    Pair(const Key& k, const Value& v) : key(k), value(v) {}


    friend ostream& operator<<(ostream& os, const Pair& p) {
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
    HashTable()
        : buckets(nullptr), loadFactor(0.0f), size(0), capacity(5) {
        init();
    }

    explicit HashTable(int cap)
        : buckets(nullptr), loadFactor(0.0f), size(0), capacity(cap/100) {
        if (capacity <= 0) {
            capacity = 5;
        }
        init();
    }

    ~HashTable() {
        clean();
        gmp_randclear(state);
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
            cerr << "Error, key " << key << " is already in the table!" << endl;
        }
    }

    void remove(const Key& key) {
        int index = hashing(key);
        int found = searchPairByKey(buckets[index].values, key);

        if (found == -1) {
            cerr << "Error, key " << key << " is not present in table!" << endl;
            return;
        } else {
            buckets[index].values.removeByValue(Pair<Key, Value>(key, Value()));
            size--;
            loadFactor = static_cast<float>(size) / capacity;
        }
    }

    bool isPresent(const Key& key) {
        int index = hashing(key);
        return searchPairByKey(buckets[index].values, key) != -1;
    }

    Value find(const Key& key) {
        int index = hashing(key);
        NodeDL<Pair<Key, Value>>* current = buckets[index].values.head;

        while (current) {
            if (current->value.key == key) {
                return current->value.value;
            }
            current = current->next;
        }

        return Value();
    }

    void clean() {
        if (buckets) {
            delete[] buckets;
            buckets = nullptr;
        }
        size = 0;
        loadFactor = 0.0;
    }

    void print() {
        ofstream out;
        out.open("hash_table.txt");

        for (int i = 0; i < capacity; i++) {
            cout << "[" << i << "]: ";
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
    gmp_randstate_t state;

    mpz_class get_random_64() {
        mpz_class r;
        mpz_urandomb(r.get_mpz_t(), state, 64);
        return r;
    }

    void init() {
        buckets = new Bucket<Key, Value>[capacity];

        gmp_randinit_default(state);
        random_device rd;
        unsigned int = 1488;
        mpz_class seed_mpz = seed;
        gmp_randseed(state, seed_mpz.get_mpz_t());

        a = get_random_64();
        b = get_random_64();
        p = generate_safe_prime(state, 64);
    }

    int searchPairByKey(DL_list<Pair<Key, Value>>& list, const Key& key) {
        NodeDL<Pair<Key, Value>>* current = list.head;
        int index = 0;

        while (current) {
            if (current->value.key == key) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }

    friend int theLongestChain(const HashTable<mpz_class, string>& table);
    friend int theShortestChain(HashTable<mpz_class, string>& table);
};
