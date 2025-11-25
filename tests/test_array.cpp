// Copyright message
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include "../include/myArray.hpp"


// вспомогающий метод для тестирования вывода cout
std::string capturePtint(const myArray<int>& arr) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    arr.print();
    std::cout.rdbuf(old);
    return buffer.str();
}

// тесты базовых операций

TEST(ArrayTest, NewArrayEmpty) {
    myArray<int> arr;
    EXPECT_EQ(arr.getSize(), 0);
}

TEST(ArrayTest, NewArrayInitialCapacity) {
    myArray<int> arr;
    EXPECT_EQ(arr.getCapacity(), 4);
}

TEST(ArrayTest, CapacityIncreasing) {
    myArray<int> arr;
    for (int i = 0; i < 6; i++) {
        arr.insert(i, i+1);
    }

    EXPECT_EQ(arr.getCapacity(), 8);
}

TEST(ArrayTest, CorrectCleaning) {
    myArray<int> arr;
    arr.insert(0, 1);
    arr.insert(1, 2);
    arr.clean();

    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_EQ(arr.getCapacity(), 0);
}

TEST(ArrayTest, InsertIncreasesSize) {
    myArray<int> arr;
    arr.insert(0, 1);
    EXPECT_EQ(arr.getSize(), 1);
    arr.insert(1, 2);
    EXPECT_EQ(arr.getSize(), 2);
}

TEST(ArrayTest, pushBackIncreaseSize) {
    myArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    EXPECT_EQ(arr.getSize(), 3);
}

TEST(ArrayTest, RemovesElement) {
    myArray<int> arr;
    for (int i = 0; i < 10; i++) {
        arr.push_back(i+1);
    }

    arr.remove(0);
    arr.remove(1);

    EXPECT_EQ(arr.getSize(), 8);
}

TEST(ArrayTest, SwappingElements) {
    myArray<int> arr;
    arr.insert(0, 5);
    arr.insert(1, 10);

    arr.swapElements(0, 10);
    arr.swapElements(1, 5);

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 5);
}

TEST(ArrayTest, ElementShiftOnInsertInMiddle) {
    myArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);

    arr.insert(1, 10);

    EXPECT_EQ(arr.getSize(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 3);
}

// тесты операторов и конструкторов

TEST(ArrayTest, CopyingConstructorWorks) {
    myArray<int> arr1;
    arr1.push_back(99);
    arr1.push_back(100);

    myArray<int> arr2(arr1);

    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2[1], 100);
}

TEST(ArrayTest, AssignmentOperatorWorks) {
    myArray<int> arr1;
    arr1.push_back(10);
    arr1.push_back(20);

    myArray<int> arr2;
    arr2 = arr1;

    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2[1], 20);
}

// тесты исключений и проверок

TEST(ArrayTest, CapacityIncreasingWorks) {
    myArray<int> arr(-10);

    EXPECT_EQ(arr.getCapacity(), 4);
    EXPECT_EQ(arr.getSize(), 0);
}

TEST(ArrayTest, OperatorColomsThrowsException) {
    myArray<int> arr;
    arr.push_back(10);

    EXPECT_THROW(arr[-1], std::out_of_range);
}

TEST(ArrayTest, AssignmentOnTheSameArrayReturnsThis) {
    myArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);

    arr = arr;

    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(ArrayTest, InsertThrowsException) {
    myArray<int> arr;
    EXPECT_THROW(arr.insert(-10, 10), std::out_of_range);
}

TEST(ArrayTest, RemoveThrowsException) {
    myArray<int> arr;
    EXPECT_THROW(arr.remove(-10), std::out_of_range);
}

TEST(ArrayTest, SwapElementsThrowsException) {
    myArray<int> arr;
    EXPECT_THROW(arr.swapElements(-10, 10), std::out_of_range);
}

// тесты вывода

TEST(ArrayTest, PrintDoesNotCrash) {
    myArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);

    std::string out = capturePtint(arr);
    EXPECT_FALSE(out.empty());
}
