// Copyright
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

#include "../include/CBT.hpp"

static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

//COMPLETE BINARY TREE (CBT)

static void BM_CBT_Insert(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        CompleteBinaryTree<int> tree;
        for (size_t i = 0; i < n; i++) {
            tree.insert(data[i]);
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_CBT_Insert)->Arg(1000)->Arg(5000)->Arg(10000)->Arg(50000);

static void BM_CBT_RemoveLast(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        state.PauseTiming();
        CompleteBinaryTree<int> tree;
        for (size_t i = 0; i < n; i++) {
            tree.insert(data[i]);
        }
        state.ResumeTiming();

        for (size_t i = 0; i < n; i++) {
            tree.removeLast();
        }

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_CBT_RemoveLast)->Arg(1000)->Arg(5000)->Arg(10000);

static void BM_CBT_GetRoot(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    CompleteBinaryTree<int> tree;
    for (size_t i = 0; i < n; i++) {
        tree.insert(data[i]);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(tree.getRoot());
    }

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_CBT_GetRoot)->Arg(1000)->Arg(10000)->Arg(50000);

static void BM_CBT_Copy(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    CompleteBinaryTree<int> original;
    for (size_t i = 0; i < n; i++) {
        original.insert(data[i]);
    }

    for (auto _ : state) {
        CompleteBinaryTree<int> copy(original);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_CBT_Copy)->Arg(1000)->Arg(5000)->Arg(10000);
