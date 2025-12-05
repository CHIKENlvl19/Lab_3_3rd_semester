#include <benchmark/benchmark.h>
#include "../include/HashTableOA.hpp"
#include <random>
#include <vector>

static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

// ------------------------------
// INSERT BENCHMARK
// ------------------------------
static void BM_HashTable_Insert(benchmark::State& state) {
    const size_t capacity = state.range(0);
    auto data = generate_random_ints(capacity);

    for (auto _ : state) {
        HashTableOA<int, int> table(capacity);

        for (size_t i = 0; i < capacity; i++) {
            table.insert(data[i], i);
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * capacity);
}

BENCHMARK(BM_HashTable_Insert)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

// ------------------------------
// FIND BENCHMARK
// ------------------------------
static void BM_HashTable_Find(benchmark::State& state) {
    const size_t capacity = state.range(0);

    auto data = generate_random_ints(capacity);
    HashTableOA<int, int> table(capacity);

    for (size_t i = 0; i < capacity; i++) {
        table.insert(data[i], i);
    }

    for (auto _ : state) {
        for (size_t i = 0; i < capacity; i++) {
            benchmark::DoNotOptimize(table.find(data[i]));
        }
    }

    state.SetItemsProcessed(state.iterations() * capacity);
}

BENCHMARK(BM_HashTable_Find)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

// ------------------------------
// REMOVE BENCHMARK
// ------------------------------
static void BM_HashTable_Remove(benchmark::State& state) {
    const size_t capacity = state.range(0);
    auto data = generate_random_ints(capacity);

    // 1. Создаем таблицу ОДИН РАЗ
    HashTableOA<int, int> table(capacity);

    for (auto _ : state) {
        // 2. Ставим на паузу таймер для подготовки
        state.PauseTiming(); 
        table.clean(); // Наш новый быстрый метод
        for (size_t i = 0; i < capacity; i++) {
            table.insert(data[i], i);
        }
        state.ResumeTiming();

        // 3. Замеряем только удаление
        for (size_t i = 0; i < capacity; i++) {
            table.remove(data[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * capacity);
}

BENCHMARK(BM_HashTable_Remove)->Arg(1000)->Arg(5000)->Arg(10000);
