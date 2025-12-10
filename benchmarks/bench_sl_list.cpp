// Copyright
#include <benchmark/benchmark.h>
#include "../include/SL_List.hpp"
#include <vector>
#include <random>

// Вспомогательная генерация случайных чисел
static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

// pushFront
static void BM_SLL_PushFront(benchmark::State& state) {
    const size_t n = state.range(0);
    auto values = generate_random_ints(n);

    for (auto _ : state) {
        SinglyLinkedList<int> list;

        for (size_t i = 0; i < n; ++i)
            list.pushFront(values[i]);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_SLL_PushFront)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// pushBack
static void BM_SLL_PushBack(benchmark::State& state) {
    const size_t n = state.range(0);
    auto values = generate_random_ints(n);

    for (auto _ : state) {
        SinglyLinkedList<int> list;

        for (size_t i = 0; i < n; ++i)
            list.pushBack(values[i]);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_SLL_PushBack)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// popFront
static void BM_SLL_PopFront(benchmark::State& state) {
    const size_t n = state.range(0);

    for (auto _ : state) {
        SinglyLinkedList<int> list;

        for (size_t i = 0; i < n; ++i)
            list.pushBack(i);

        for (size_t i = 0; i < n; ++i)
            list.popFront();

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_SLL_PopFront)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// Full Scan (итерация через popFront + pushBack имитация)
static void BM_SLL_LinearScan(benchmark::State& state) {
    const size_t n = state.range(0);

    SinglyLinkedList<int> base;
    for (size_t i = 0; i < n; ++i) base.pushBack(1);

    for (auto _ : state) {
        SinglyLinkedList<int> list = base;

        int sum = 0;
        while (!list.empty()) {
            sum += list.front();
            list.popFront();
        }

        benchmark::DoNotOptimize(sum);
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_SLL_LinearScan)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// Construct benchmark
static void BM_SLL_Construct(benchmark::State& state) {
    for (auto _ : state) {
        SinglyLinkedList<int> list;
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_SLL_Construct);

// Copy benchmark (копирование большого списка)
static void BM_SLL_Copy(benchmark::State& state) {
    const size_t n = state.range(0);

    SinglyLinkedList<int> base;
    for (size_t i = 0; i < n; ++i) base.pushBack(i);

    for (auto _ : state) {
        SinglyLinkedList<int> copy = base;
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_SLL_Copy)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

