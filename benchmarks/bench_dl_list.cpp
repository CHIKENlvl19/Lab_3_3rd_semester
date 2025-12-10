// Copyright
#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include "../include/DL_List.hpp"

static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

static void BM_DLList_AddHead(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        DL_list<int> list;
        for (size_t i = 0; i < n; i++) {
            list.addHead(data[i]);
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_DLList_AddHead)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_DLList_AddTail(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        DL_list<int> list;
        for (size_t i = 0; i < n; i++) {
            list.addTail(data[i]);
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_DLList_AddTail)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_DLList_SearchByValue(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    DL_list<int> list;
    for (size_t i = 0; i < n; i++) {
        list.addTail(data[i]);
    }

    for (auto _ : state) {
        for (size_t i = 0; i < n; i++) {
            benchmark::DoNotOptimize(list.searchByValue(data[i]));
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_DLList_SearchByValue)->Arg(1000)->Arg(5000)->Arg(10000);

static void BM_DLList_RemoveByValue(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        state.PauseTiming();
        DL_list<int> list;
        for (size_t i = 0; i < n; i++) {
            list.addTail(data[i]);
        }
        state.ResumeTiming();

        for (size_t i = 0; i < n; i++) {
            list.removeByValue(data[i]);
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_DLList_RemoveByValue)->Arg(1000)->Arg(5000)->Arg(10000);

static void BM_DLList_Copy(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    DL_list<int> original;
    for (size_t i = 0; i < n; i++) {
        original.addTail(data[i]);
    }

    for (auto _ : state) {
        DL_list<int> copy(original);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_DLList_Copy)->Arg(1000)->Arg(5000)->Arg(10000);
