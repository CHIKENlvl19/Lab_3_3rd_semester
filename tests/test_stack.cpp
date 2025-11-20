// Copyright message
#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/myStack.hpp"

TEST(StackTest, PushIncreasesSize) {
    Stack<int> s;
    s.push(10);
    s.push(20);
    EXPECT_EQ(s.getSize(), 2);
}

TEST(StackTest, PopDecreasesSize) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.pop();
    EXPECT_EQ(s.getSize(), 1);
}

TEST(StackTest, PopOnEmptyThrows) {
    Stack<int> s;
    EXPECT_THROW(s.pop(), std::underflow_error);
}

TEST(StackTest, PeekReturnsTop) {
    Stack<int> s;
    s.push(5);
    s.push(9);
    EXPECT_EQ(s.peek(), 9);
}

TEST(StackTest, PeekOnEmptyThrows) {
    Stack<int> s;
    EXPECT_THROW(s.peek(), std::underflow_error);
}

TEST(StackTest, CopyConstructorWorks) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    Stack<int> c(s);
    EXPECT_EQ(c.getSize(), s.getSize());
    EXPECT_EQ(c.peek(), s.peek());
}

TEST(StackTest, AssignmentOperatorWorks) {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);

    Stack<int> s2;
    s2.push(9);

    s2 = s1;
    EXPECT_EQ(s2.getSize(), 2);
    EXPECT_EQ(s2.peek(), 2);
}

TEST(StackTest, DeepCopyCheck) {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);

    Stack<int> s2(s1);
    s1.pop();

    EXPECT_EQ(s2.getSize(), 2);
    EXPECT_EQ(s2.peek(), 2);
}

TEST(StackTest, MaxStackOverflow) {
    Stack<int> s;
    for (int i = 0; i < MAX_STACK_SIZE; ++i) s.push(i);

    EXPECT_THROW(s.push(999), std::overflow_error);
}

TEST(StackTest, PrintDoesNotCrash) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.print();
}
