#include <gtest/gtest.h>
#include <sstream>
#include "../include/DL_List.hpp"

// Проверка базовой функциональности
TEST(DLList, InitiallyEmpty) {
    DL_list<int> lst;
    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.getSize(), 0);
}

TEST(DLList, AddHeadAndAddTail) {
    DL_list<int> lst;
    lst.addHead(1);
    lst.addTail(2);
    lst.addHead(0);

    EXPECT_EQ(lst.getSize(), 3);
    EXPECT_EQ(lst.searchByValue(0), 0);
    EXPECT_EQ(lst.searchByValue(1), 1);
    EXPECT_EQ(lst.searchByValue(2), 2);
}

TEST(DLList, AddAfterAndAddBefore) {
    DL_list<int> lst;
    lst.addTail(1);
    lst.addTail(2);
    lst.addTail(3);

    lst.addAfter(1, 99);  // после 2
    EXPECT_EQ(lst.searchByValue(99), 2);

    lst.addBefore(0, 777); // перед 1
    EXPECT_EQ(lst.searchByValue(777), 0);
}

TEST(DLList, AddBeforeMiddle) {
    DL_list<int> lst;
    lst.addTail(1);
    lst.addTail(2);
    lst.addTail(4); // список: 1,2,4

    lst.addBefore(2, 3); // вставка перед 4

    EXPECT_EQ(lst.getSize(), 4);
    std::ostringstream out;
    lst.print(out);
    EXPECT_EQ(out.str(), "[1 <-> 2 <-> 3 <-> 4]\n");
}


TEST(DLList, RemoveByValue) {
    DL_list<int> lst;
    lst.addTail(1);
    lst.addTail(2);
    lst.addTail(1);
    lst.removeByValue(1);

    EXPECT_EQ(lst.getSize(), 1);
    EXPECT_EQ(lst.searchByValue(2), 0);
}

// Проверка печати
TEST(DLList, PrintAndPrintBackwards) {
    DL_list<int> lst;
    lst.addTail(1);
    lst.addTail(2);
    lst.addTail(3);

    std::ostringstream oss;
    lst.print(oss);
    EXPECT_EQ(oss.str(), "[1 <-> 2 <-> 3]\n");

    std::ostringstream oss2;
    lst.printBackwards(oss2);
    EXPECT_EQ(oss2.str(), "[3 <-> 2 <-> 1]\n");
}

// Конструктор копирования
TEST(DLList, CopyConstructor) {
    DL_list<int> lst;
    lst.addTail(10);
    lst.addTail(20);

    DL_list<int> copy(lst);
    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(copy.searchByValue(10), 0);
    EXPECT_EQ(copy.searchByValue(20), 1);

    // Изменения оригинала не влияют на копию
    lst.removeByValue(10);
    EXPECT_EQ(copy.searchByValue(10), 0);
}

// Оператор присваивания
TEST(DLList, CopyAssignment) {
    DL_list<int> a;
    a.addTail(1);
    a.addTail(2);

    DL_list<int> b;
    b = a;

    EXPECT_EQ(b.getSize(), 2);
    EXPECT_EQ(b.searchByValue(1), 0);

    a.removeByValue(2);
    EXPECT_EQ(b.searchByValue(2), 1);  // deep copy
}

// исключения при копировании
struct Evil {
    int x;
    static bool shouldThrow;
    explicit Evil(int x) : x(x) {}
    Evil(const Evil& other) : x(other.x) {
        if (shouldThrow) throw std::runtime_error("copy failed");
    }
};

bool Evil::shouldThrow = false;

TEST(DLList, CopyConstructorThrows) {
    DL_list<Evil> lst;
    Evil::shouldThrow = false;
    lst.addTail(Evil(1));  // безопасно
    lst.addTail(Evil(2));  // безопасно

    Evil::shouldThrow = true;  // теперь копирование бросает

    EXPECT_THROW({
        DL_list<Evil> Copy(lst);
    }, std::runtime_error);

    // исходный список остаётся в корректном состоянии
    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_FALSE(lst.empty());
}

TEST(DLList, CopyConstructorThrowsInsideLoop) {
    DL_list<Evil> lst;
    Evil::shouldThrow = false;
    lst.addTail(Evil(1));
    lst.addTail(Evil(2)); // безопасно

    Evil::shouldThrow = true; // второй бросит

    EXPECT_THROW({
        DL_list<Evil> Copy(lst);
    }, std::runtime_error);

    // исходный список остаётся в корректном состоянии
    EXPECT_EQ(lst.getSize(), 2);
    EXPECT_FALSE(lst.empty());
}
