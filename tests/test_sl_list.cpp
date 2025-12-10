// Copyright message
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "../include/SL_List.hpp"

// вспомогательная функция для проверки вывода
std::string capturePrint(const SinglyLinkedList<int>& lst) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    lst.print();
    std::cout.rdbuf(old);
    return buffer.str();
}


// Базовая инициализация

TEST(SinglyLinkedListTest, InitiallyEmpty) {
    SinglyLinkedList<int> lst;
    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.getSize(), 0);
}


// pushFront

TEST(SinglyLinkedListTest, PushFrontWorks) {
    SinglyLinkedList<int> lst;
    lst.pushFront(10);
    EXPECT_FALSE(lst.empty());
    EXPECT_EQ(lst.getSize(), 1);
    EXPECT_EQ(lst.front(), 10);

    lst.pushFront(20);
    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_EQ(lst.front(), 20);
}


// pushBack

TEST(SinglyLinkedListTest, PushBackWorks) {
    SinglyLinkedList<int> lst;

    lst.pushBack(5);
    EXPECT_EQ(lst.getSize(), 1);
    EXPECT_EQ(lst.front(), 5);

    lst.pushBack(10);
    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_EQ(lst.front(), 5);
}


// popFront

TEST(SinglyLinkedListTest, PopFrontWorks) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);

    lst.popFront();
    EXPECT_EQ(lst.front(), 2);
    EXPECT_EQ(lst.getSize(), 1);

    lst.popFront();
    EXPECT_TRUE(lst.empty());
}

TEST(SinglyLinkedListTest, PopFrontThrowsOnEmpty) {
    SinglyLinkedList<int> lst;
    EXPECT_THROW(lst.popFront(), std::underflow_error);
}


// popBack

TEST(SinglyLinkedListTest, PopBackOnSingleElement) {
    SinglyLinkedList<int> lst;
    lst.pushBack(42);

    lst.popBack();

    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.getSize(), 0);
}

TEST(SinglyLinkedListTest, PopBackWorks) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);
    lst.pushBack(3);

    lst.popBack();

    EXPECT_EQ(lst.getSize(), 2);

    // front = 1, next = 2
    lst.popFront();
    EXPECT_EQ(lst.front(), 2);
}

TEST(SinglyLinkedListTest, PopBackThrowsOnEmpty) {
    SinglyLinkedList<int> lst;
    EXPECT_THROW(lst.popBack(), std::underflow_error);
}

TEST(SinglyLinkedListTest, PopBackRemovesLastFromThreeElementList) {
    SinglyLinkedList<int> lst;

    lst.pushBack(10);
    lst.pushBack(20);
    lst.pushBack(30);

    lst.popBack();

    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_FALSE(lst.empty());
    EXPECT_EQ(lst.front(), 10);

    // Проверим print(), чтобы убедиться, что значение 20 осталось крайним
    std::string out = capturePrint(lst);
    EXPECT_EQ(out, "Head -> 10 -> 20 -> nullptr\n");
}


// copy constructor

TEST(SinglyLinkedListTest, CopyConstructorCreatesSeparateCopy) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);
    lst.pushBack(3);

    SinglyLinkedList<int> copy(lst);

    EXPECT_EQ(copy.getSize(), 3);

    // Проверяем порядок
    EXPECT_EQ(copy.front(), 1);

    lst.popFront();  // меняем оригинал
    EXPECT_EQ(copy.front(), 1);  // копия не изменилась
}


// operator=

TEST(SinglyLinkedListTest, AssignmentOperatorWorks) {
    SinglyLinkedList<int> a;
    a.pushBack(10);
    a.pushBack(20);

    SinglyLinkedList<int> b;
    b.pushBack(1);

    b = a;
    EXPECT_EQ(b.getSize(), 2);
    EXPECT_EQ(b.front(), 10);

    a.popFront();  // изменяем оригинал
    EXPECT_EQ(b.front(), 10);  // копия не изменилась
}

TEST(SinglyLinkedListTest, AssignmentSelf) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);

    lst = lst;

    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_EQ(lst.front(), 1);
}


// clear

TEST(SinglyLinkedListTest, ClearWorks) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);
    lst.pushBack(3);

    lst.clear();

    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.getSize(), 0);

    // убедимся, что можно снова добавлять
    lst.pushBack(99);
    EXPECT_FALSE(lst.empty());
    EXPECT_EQ(lst.front(), 99);
}


// empty and getSize

TEST(SinglyLinkedListTest, EmptyAndSizeCorrect) {
    SinglyLinkedList<int> lst;
    EXPECT_TRUE(lst.empty());

    lst.pushBack(10);
    EXPECT_FALSE(lst.empty());
    EXPECT_EQ(lst.getSize(), 1);

    lst.pushFront(20);
    EXPECT_EQ(lst.getSize(), 2);
}


// front() exceptions

TEST(SinglyLinkedListTest, FrontThrowsOnEmpty) {
    SinglyLinkedList<int> lst;
    EXPECT_THROW(lst.front(), std::underflow_error);
}


// тесты печати

TEST(SinglyLinkedListTest, PrintEmptyList) {
    SinglyLinkedList<int> lst;
    std::string out = capturePrint(lst);

    EXPECT_EQ(out, "Head -> nullptr\n");
}

TEST(SinglyLinkedListTest, PrintSingleElement) {
    SinglyLinkedList<int> lst;
    lst.pushBack(5);

    std::string out = capturePrint(lst);

    EXPECT_EQ(out, "Head -> 5 -> nullptr\n");
}

TEST(SinglyLinkedListTest, PrintMultipleElements) {
    SinglyLinkedList<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);
    lst.pushBack(3);

    std::string out = capturePrint(lst);

    EXPECT_EQ(out, "Head -> 1 -> 2 -> 3 -> nullptr\n");
}

