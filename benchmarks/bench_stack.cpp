// Copyright message
#include <benchmark/benchmark.h>
#include <vector>
#include "../include/myStack.hpp"

static void BM_Stack_Push(benchmark::State& state) {
    int N = state.range(0);
    for (auto _ : state) {
        Stack<int> st;
        for (int i = 0; i < N; i++) {
            st.push(i);
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Stack_Push)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);

static void BM_Stack_Pop(benchmark::State& state) {
    int N = state.range(0);

    for (auto _ : state) {
        Stack<int> st;
        for (int i = 0; i < N; i++) st.push(i);

        for (int i = 0; i < N; i++) {
            st.pop();
        }
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Stack_Pop)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);

static void BM_Stack_Peek(benchmark::State& state) {
    int N = state.range(0);

    Stack<int> st;
    for (int i = 0; i < N; i++) st.push(i);

    for (auto _ : state) {
        int x = st.peek();
        benchmark::DoNotOptimize(x);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Stack_Peek)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);

static void BM_Stack_CopyCtor(benchmark::State& state) {
    int N = state.range(0);

    Stack<int> st;
    for (int i = 0; i < N; i++) st.push(i);

    for (auto _ : state) {
        Stack<int> copy(st);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Stack_CopyCtor)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);

static void BM_Stack_Assignment(benchmark::State& state) {
    int N = state.range(0);

    Stack<int> st;
    for (int i = 0; i < N; i++) st.push(i);

    for (auto _ : state) {
        Stack<int> st2;
        st2 = st;
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());

}

BENCHMARK(BM_Stack_Assignment)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);

static void BM_Stack_FullCycle(benchmark::State& state) {
    int N = state.range(0);

    for (auto _ : state) {
        Stack<int> st;
        for (int i = 0; i < N; i++) st.push(i);
        for (int i = 0; i < N; i++) st.pop();
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(state.iterations());

}

BENCHMARK(BM_Stack_FullCycle)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(5000);
