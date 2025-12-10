// Copyright
#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include "../include/HashTableChains.hpp"

static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

// HASHTABLE CHAINS

static void BM_HashTableChains_Insert(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        HashTable<int, int> table(n * 2);
        for (size_t i = 0; i < n; i++) {
            table.insert(data[i], i);
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashTableChains_Insert)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_HashTableChains_Find(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    HashTable<int, int> table(n * 2);
    for (size_t i = 0; i < n; i++) {
        table.insert(data[i], i);
    }

    for (auto _ : state) {
        for (size_t i = 0; i < n; i++) {
            benchmark::DoNotOptimize(table.find(data[i]));
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashTableChains_Find)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_HashTableChains_IsPresent(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    HashTable<int, int> table(n * 2);
    for (size_t i = 0; i < n; i++) {
        table.insert(data[i], i);
    }

    for (auto _ : state) {
        for (size_t i = 0; i < n; i++) {
            benchmark::DoNotOptimize(table.isPresent(data[i]));
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_HashTableChains_IsPresent)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_HashTableChains_Remove(benchmark::State& state) {
    const size_t n = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashTable<int, int> table(n * 2);

        // Вставляем УНИКАЛЬНЫЕ ключи: 0, 1, 2, ..., n-1
        for (size_t i = 0; i < n; i++) {
            table.insert(i, i);
        }
        state.ResumeTiming();

        // Удаляем те же ключи
        for (size_t i = 0; i < n; i++) {
            table.remove(i);
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}


BENCHMARK(BM_HashTableChains_Remove)->Arg(1000)->Arg(5000)->Arg(10000);
