#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <random>

static void BM_Array_Read(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n);
    for (auto& x : arr) x = 42;

    for (auto _ : state) {
        size_t idx = state.iterations() % n;
        benchmark::DoNotOptimize(arr[idx]);
    }
}

static void BM_Array_Write(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n);

    for (auto _ : state) {
        size_t idx = state.iterations() % n;
        arr[idx] = 123;
        benchmark::ClobberMemory();
    }
}

static void BM_Array_LinearScan(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n, 1);

    for (auto _ : state) {
        int sum = 0;
        for (size_t i = 0; i < n; i++) sum += arr[i];
        benchmark::DoNotOptimize(sum);
    }
}

static void BM_Array_BinarySearch(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n);
    for (size_t i = 0; i < n; ++i) arr[i] = i;

    for (auto _ : state) {
        int key = n / 2;
        auto it = std::lower_bound(arr.begin(), arr.end(), key);
        benchmark::DoNotOptimize(it);
    }
}

static void BM_Array_InsertMiddle(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n, 1);

    for (auto _ : state) {
        std::vector<int> tmp = arr; // делаем копию для честного теста
        tmp.insert(tmp.begin() + tmp.size() / 2, 123);
        benchmark::ClobberMemory();
    }
}

static void BM_Array_EraseMiddle(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> arr(n, 1);

    for (auto _ : state) {
        std::vector<int> tmp = arr;
        tmp.erase(tmp.begin() + tmp.size() / 2);
        benchmark::ClobberMemory();
    }
}


// ---------------------
// РЕГИСТРАЦИЯ БЕНЧМАРКОВ
// ---------------------

BENCHMARK(BM_Array_Read)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);
BENCHMARK(BM_Array_Write)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);
BENCHMARK(BM_Array_LinearScan)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);
BENCHMARK(BM_Array_BinarySearch)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);
BENCHMARK(BM_Array_InsertMiddle)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);
BENCHMARK(BM_Array_EraseMiddle)->Arg(1<<10)->Arg(1<<15)->Arg(1<<20);

BENCHMARK_MAIN();
