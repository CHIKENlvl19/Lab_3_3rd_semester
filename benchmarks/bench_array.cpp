// Copyright message
#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <random>

// вспомогательная генерация случайных чисел
static std::vector<int> generate_random_ints(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1'000'000);
    std::vector<int> v(n);
    for (size_t i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

// READ BENCHMARK
static void BM_Array_Read(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        size_t idx = state.iterations() % n;
        benchmark::DoNotOptimize(data[idx]);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Array_Read)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// WRITE BENCHMARK
static void BM_Array_Write(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = generate_random_ints(n);

    for (auto _ : state) {
        size_t idx = state.iterations() % n;
        data[idx] = 123;
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Array_Write)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// LINEAR SCAN BENCHMARK
static void BM_Array_LinearScan(benchmark::State& state) {
    const size_t n = state.range(0);
    auto data = std::vector<int>(n, 1);

    for (auto _ : state) {
        int sum = 0;
        for (size_t i = 0; i < n; i++) sum += data[i];
        benchmark::DoNotOptimize(sum);
    }

    state.SetItemsProcessed(state.iterations() * n);
}

BENCHMARK(BM_Array_LinearScan)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// BINARY SEARCH BENCHMARK
static void BM_Array_BinarySearch(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> data(n);
    for (size_t i = 0; i < n; ++i) data[i] = i;

    for (auto _ : state) {
        int key = n / 2;
        auto it = std::lower_bound(data.begin(), data.end(), key);
        benchmark::DoNotOptimize(it);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Array_BinarySearch)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// INSERT MIDDLE BENCHMARK
static void BM_Array_InsertMiddle(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> base(n, 1);

    for (auto _ : state) {
        std::vector<int> data = base;
        data.insert(data.begin() + data.size() / 2, 123);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Array_InsertMiddle)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

// ERASE MIDDLE BENCHMARK
static void BM_Array_EraseMiddle(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> base(n, 1);

    for (auto _ : state) {
        std::vector<int> data = base;
        data.erase(data.begin() + data.size() / 2);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Array_EraseMiddle)->Arg(1 << 10)->Arg(1 << 15)->Arg(1 << 20);

BENCHMARK_MAIN();
