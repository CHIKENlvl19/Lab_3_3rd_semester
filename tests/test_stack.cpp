// Copyright message
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include "../include/myStack.hpp"


// вспомогающий метод для тестирования вывода cout
std::string capturePrint(const Stack<int>& st) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    st.print();
    std::cout.rdbuf(old);
    return buffer.str();
}

// тесты базовых операций

TEST(StackTest, NewStackIsEmpty) {
    Stack<int> st;
    EXPECT_EQ(st.getSize(), 0);
    EXPECT_THROW(st.peek(), std::underflow_error);
}

TEST(StackTest, PushIncreasesSize) {
    Stack<int> st;
    st.push(10);
    EXPECT_EQ(st.getSize(), 1);
    st.push(20);
    EXPECT_EQ(st.getSize(), 2);
}

TEST(StackTest, PopDecreasesSize) {
    Stack<int> st;
    st.push(1);
    st.push(2);
    st.pop();
    EXPECT_EQ(st.getSize(), 1);
}

TEST(StackTest, PopOnEmptyThrows) {
    Stack<int> st;
    EXPECT_THROW(st.pop(), std::underflow_error);
}

TEST(StackTest, PeekReturnsTop) {
    Stack<int> st;
    st.push(5);
    st.push(10);
    EXPECT_EQ(st.peek(), 10);
}

TEST(StackTest, PeekOnEmptyThrows) {
    Stack<int> st;
    EXPECT_THROW(st.peek(), std::underflow_error);
}

// тесты семантики копирования

TEST(StackTest, CopyConstructorWorks) {
    Stack<int> st;
    st.push(1);
    st.push(2);

    Stack<int> copy(st);

    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(copy.peek(), 2);
}

TEST(StackTest, AssignmentOperatorWorks) {
    Stack<int> st;
    st.push(7);
    st.push(9);

    Stack<int> st2;
    st2 = st;

    EXPECT_EQ(st2.getSize(), 2);
    EXPECT_EQ(st2.peek(), 9);
}

TEST(StackTest, SelfAssignmentDoesNotBreak) {
    Stack<int> st;
    st.push(1);
    st = st;   // self-assignment

    EXPECT_EQ(st.getSize(), 1);
    EXPECT_EQ(st.peek(), 1);
}

TEST(StackTest, DeepCopyCheck) {
    Stack<int> st;
    st.push(1);
    st.push(2);

    Stack<int> copy(st);
    copy.pop();

    EXPECT_NE(copy.getSize(), st.getSize());
    EXPECT_EQ(st.getSize(), 2);
}

// тесты границ (пределов)

TEST(StackTest, MaxStackOverflow) {
    Stack<int> st;

    for (int i = 0; i < MAX_STACK_SIZE; i++) {
        st.push(i);
    }

    EXPECT_THROW(st.push(100), std::overflow_error);
}

TEST(StackTest, ManyOperationsSequence) {
    Stack<int> st;

    for (int i = 0; i < 200; i++) {
        st.push(i);
    }
    for (int i = 0; i < 150; i++) {
        st.pop();
    }

    EXPECT_EQ(st.getSize(), 50);
    EXPECT_EQ(st.peek(), 199 - 150);
}

TEST(StackCopyConstructor, CopyEmptyStack) {
    Stack<int> s1;
    Stack<int> s2(s1);
    EXPECT_EQ(s2.getSize(), 0);
}

TEST(StackCopyConstructor, CopyNonEmptyStack) {
    Stack<int> s1;
    s1.push(10);
    s1.push(20);

    Stack<int> s2(s1);
    EXPECT_EQ(s2.getSize(), s1.getSize());
    EXPECT_EQ(s2.peek(), s1.peek());
}

// тесты вывода

TEST(StackTest, PrintDoesNotCrash) {
    Stack<int> st;
    st.push(1);
    st.push(2);

    std::string out = capturePrint(st);
    EXPECT_FALSE(out.empty());
}

TEST(StackTest, PrintEmptyStack) {
    Stack<int> st;
    std::string out = capturePrint(st);
    EXPECT_NE(out.find("empty"), std::string::npos);
}

// тесты на разных шаблонах

TEST(StackTest, WorksWithStrings) {
    Stack<std::string> st;
    st.push("hello");
    st.push("world");

    EXPECT_EQ(st.peek(), "world");

    st.pop();
    EXPECT_EQ(st.peek(), "hello");
}

TEST(StackTest, WorksWithCustomType) {
    struct TestObj {
        int x;
        explicit TestObj(int v = 0) : x(v) {}
        bool operator==(const TestObj& o) const { return x == o.x; }
    };

    Stack<TestObj> st;
    st.push(TestObj(10));
    st.push(TestObj(20));

    EXPECT_EQ(st.peek().x, 20);
}
