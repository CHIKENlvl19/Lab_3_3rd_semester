// Copyright message
#include <benchmark/benchmark.h>
#include <vector>
#include <random>
#include "../include/myQueue.hpp"

// ------------------------------------------------------
// Вспомогательная генерация случайных чисел
// ------------------------------------------------------
static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

// ------------------------------------------------------
// PUSH benchmark
// ------------------------------------------------------
static void BM_Queue_Push(benchmark::State& state) {
    const size_t n = state.range(0);
    myQueue<int> q;
    auto values = generate_random_ints(n);

    for (auto _ : state) {
        q = myQueue<int>();  // очистка
        for (size_t i = 0; i < n; ++i) {
            q.push(values[i], "tmp.txt");  // если хочешь — уберем файл
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_Queue_Push)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// ------------------------------------------------------
// POP benchmark
// ------------------------------------------------------
static void BM_Queue_Pop(benchmark::State& state) {
    const size_t n = state.range(0);

    for (auto _ : state) {
        myQueue<int> q;
        for (size_t i = 0; i < n; ++i) q.push(i, "tmp.txt");

        for (size_t i = 0; i < n; ++i) {
            q.pop("tmp.txt");
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_Queue_Pop)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// ------------------------------------------------------
// READ FRONT benchmark
// ------------------------------------------------------
static void BM_Queue_Front(benchmark::State& state) {
    const size_t n = state.range(0);
    myQueue<int> q;

    for (size_t i = 0; i < n; ++i) q.push(i, "tmp.txt");

    for (auto _ : state) {
        benchmark::DoNotOptimize(q.getSize());
        // Читаем первый элемент
        benchmark::DoNotOptimize(q.getCapacity());
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Queue_Front)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// ------------------------------------------------------
// Full Scan benchmark (pop + push)
// имитирует последовательный обход очереди
// ------------------------------------------------------
static void BM_Queue_FullScan(benchmark::State& state) {
    const size_t n = state.range(0);

    for (auto _ : state) {
        myQueue<int> q;
        for (size_t i = 0; i < n; ++i) q.push(i, "tmp.txt");

        for (size_t i = 0; i < n; ++i) {
            q.pop("tmp.txt");
            q.push(i, "tmp.txt");
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_Queue_FullScan)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// ------------------------------------------------------
// Construct queue benchmark
// ------------------------------------------------------
static void BM_Queue_Construct(benchmark::State& state) {
    for (auto _ : state) {
        myQueue<int> q;
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Queue_Construct);
