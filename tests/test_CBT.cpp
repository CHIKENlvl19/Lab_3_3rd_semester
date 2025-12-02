// Copyright message
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "../include/CBT.hpp"


// Перехватывает вывод в stdout во время выполнения функции f
std::string captureStdout(std::function<void()> f) {
    std::stringstream buffer;
    auto* oldBuf = std::cout.rdbuf(buffer.rdbuf());  // перенаправили cout

    f();                                            // выполняем код

    std::cout.rdbuf(oldBuf);                        // возвращаем состояние
    return buffer.str();
}


TEST(CompleteBinaryTree, DefaultConstructor) {
    CompleteBinaryTree<int> t;
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.getSize(), 0);
    EXPECT_THROW(t.getRoot(), std::underflow_error);
}

TEST(CompleteBinaryTree, InsertSingle) {
    CompleteBinaryTree<int> t;
    t.insert(10);

    EXPECT_FALSE(t.empty());
    EXPECT_EQ(t.getSize(), 1);
    EXPECT_EQ(t.getRoot(), 10);
}

TEST(CompleteBinaryTree, InsertMultiple) {
    CompleteBinaryTree<int> t;

    t.insert(1);   // root
    t.insert(2);   // left
    t.insert(3);   // right
    t.insert(4);   // 2->left
    t.insert(5);   // 2->right

    EXPECT_EQ(t.getSize(), 5);
    EXPECT_EQ(t.getRoot(), 1);
}

TEST(CompleteBinaryTree, RemoveLastFromEmpty) {
    CompleteBinaryTree<int> t;
    EXPECT_THROW(t.removeLast(), std::underflow_error);
}

TEST(CompleteBinaryTree, RemoveLastSingle) {
    CompleteBinaryTree<int> t;

    t.insert(42);
    ASSERT_EQ(t.getSize(), 1);

    t.removeLast();

    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.getSize(), 0);
    EXPECT_THROW(t.getRoot(), std::underflow_error);
}

TEST(CompleteBinaryTree, RemoveLastMultiple) {
    CompleteBinaryTree<int> t;

    t.insert(1);
    t.insert(2);
    t.insert(3);
    t.insert(4);

    ASSERT_EQ(t.getSize(), 4);

    t.removeLast();   // удалили 4

    EXPECT_EQ(t.getSize(), 3);
    EXPECT_EQ(t.getRoot(), 1);

    t.removeLast();   // удалили 3
    EXPECT_EQ(t.getSize(), 2);

    t.removeLast();   // удалили 2
    EXPECT_EQ(t.getSize(), 1);

    t.removeLast();   // удалили 1
    EXPECT_TRUE(t.empty());
}

TEST(CompleteBinaryTree, CopyConstructorSingle) {
    CompleteBinaryTree<int> t1;
    t1.insert(100);

    CompleteBinaryTree<int> t2(t1);

    EXPECT_EQ(t2.getSize(), 1);
    EXPECT_EQ(t2.getRoot(), 100);
}

TEST(CompleteBinaryTree, CopyConstructorMultiple) {
    CompleteBinaryTree<int> t1;
    t1.insert(1);
    t1.insert(2);
    t1.insert(3);
    t1.insert(4);
    t1.insert(5);

    CompleteBinaryTree<int> t2(t1);

    EXPECT_EQ(t2.getSize(), 5);
    EXPECT_EQ(t2.getRoot(), 1);
}


TEST(CompleteBinaryTree, AssignmentOperator) {
    CompleteBinaryTree<int> t1;
    t1.insert(7);
    t1.insert(8);
    t1.insert(9);

    CompleteBinaryTree<int> t2;
    t2.insert(100);

    t2 = t1;

    EXPECT_EQ(t2.getSize(), 3);
    EXPECT_EQ(t2.getRoot(), 7);

    // изменение одного не влияет на другой
    t1.insert(999);

    EXPECT_EQ(t1.getSize(), 4);
    EXPECT_EQ(t2.getSize(), 3);
}


TEST(CompleteBinaryTree, SelfAssignment) {
    CompleteBinaryTree<int> t;
    t.insert(1);
    t.insert(2);

    t = t;  // <- важно

    EXPECT_EQ(t.getSize(), 2);
    EXPECT_EQ(t.getRoot(), 1);
}


TEST(CBT_Print, EmptyTreePrint) {
    CompleteBinaryTree<int> tree;

    std::string out = captureStdout([&]() {
        tree.printLevelOrder();
    });

    EXPECT_EQ(out, "[]\n");
}

TEST(CBT_Print, SingleElementPrint) {
    CompleteBinaryTree<int> tree;
    tree.insert(42);

    std::string out = captureStdout([&]() {
        tree.printLevelOrder();
    });

    EXPECT_EQ(out, "[42]\n");
}

TEST(CBT_Print, MultipleElementsPrint) {
    CompleteBinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    std::string out = captureStdout([&]() {
        tree.printLevelOrder();
    });

    // дерево по уровням: 1, 2, 3, 4
    EXPECT_EQ(out, "[1, 2, 3, 4]\n");
}