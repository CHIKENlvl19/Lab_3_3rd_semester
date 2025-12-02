// Copyright message
#include <sstream>
#include "gtest/gtest.h"
#include "../include/DL_List.hpp"

// Вспомогательная структура, которая может выбросить исключение при копировании
struct TestStruct {
    int data;
    static bool throw_on_copy;  // cтатическая переменная для управления исключением
    explicit TestStruct(int d) : data(d) {}
    TestStruct(const TestStruct& other) : data(other.data) {
        if (throw_on_copy && data == 999) {  // условие для выброса исключения
            throw std::runtime_error("Exception in copy constructor");
        }
    }
};

bool TestStruct::throw_on_copy = true;

// тесты для конструктора по умолчанию
TEST(DLListTest, DefaultConstructor) {
    DL_list<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.getSize(), 0);
}

// тесты для добавления в начало
TEST(DLListTest, AddHead) {
    DL_list<int> list;
    list.addHead(1);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.getSize(), 1);

    list.addHead(2);
    EXPECT_EQ(list.getSize(), 2);
}

// тесты для добавления в конец
TEST(DLListTest, AddTail) {
    DL_list<int> list;
    list.addTail(1);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.getSize(), 1);

    list.addTail(2);
    EXPECT_EQ(list.getSize(), 2);
}

// тесты для добавления по индексу (addAfter)
TEST(DLListTest, AddAfter) {
    DL_list<int> list;
    list.addHead(1);
    list.addTail(3);

    list.addAfter(0, 2);
    EXPECT_EQ(list.getSize(), 3);
    // порядок должен быть [1, 2, 3]
}

TEST(DLListTest, AddAfterAtEnd) {
    DL_list<int> list;
    list.addHead(1);
    list.addAfter(1, 2);  // добавляем в конец
    EXPECT_EQ(list.getSize(), 2);
}

TEST(DLListTest, AddAfterOutOfRange) {
    DL_list<int> list;
    list.addHead(1);
    EXPECT_THROW(list.addAfter(-1, 5), std::out_of_range);
    EXPECT_THROW(list.addAfter(2, 5), std::out_of_range);
}

// тесты для добавления по индексу (addBefore)
TEST(DLListTest, AddBefore) {
    DL_list<int> list;
    list.addHead(2);
    list.addTail(3);

    list.addBefore(1, 1); // добавляем 1 перед 1-м элементом (перед 2)
    EXPECT_EQ(list.getSize(), 3);
    // порядок должен быть [1, 2, 3]
}

TEST(DLListTest, AddBeforeAtStart) {
    DL_list<int> list;
    list.addHead(2);
    list.addBefore(0, 1); // добавляем в начало
    EXPECT_EQ(list.getSize(), 2);
}

TEST(DLListTest, AddBeforeAtEnd) {
    DL_list<int> list;
    list.addHead(1);
    list.addBefore(1, 2); // добавляем перед последним
    EXPECT_EQ(list.getSize(), 2);
}

TEST(DLListTest, AddBeforeOutOfRange) {
    DL_list<int> list;
    list.addHead(1);
    EXPECT_THROW(list.addBefore(-1, 5), std::out_of_range);
    EXPECT_THROW(list.addBefore(2, 5), std::out_of_range);
}

// тесты для поиска по значению
TEST(DLListTest, SearchByValue) {
    DL_list<int> list;
    list.addHead(1);
    list.addTail(4);
    list.addTail(3);
    list.addTail(4);

    EXPECT_EQ(list.searchByValue(1), 0);
    EXPECT_EQ(list.searchByValue(4), 1); // первый индекс
    EXPECT_EQ(list.searchByValue(3), 2);
    EXPECT_EQ(list.searchByValue(5), -1); // не найдено
}

// Тесты для удаления по значению
TEST(DLListTest, RemoveByValue) {
    DL_list<int> list;
    list.addHead(7);
    list.addTail(3);
    list.addTail(7);
    list.addTail(5);

    list.removeByValue(7);  // удаляем все 7
    EXPECT_EQ(list.getSize(), 2); // остались 3 и 5

    list.removeByValue(10); // удаляем несуществующее
    EXPECT_EQ(list.getSize(), 2); // размер не изменился
}

// Тесты для очистки
TEST(DLListTest, Clear) {
    DL_list<int> list;
    list.addHead(1);
    list.addTail(2);
    list.addTail(3);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.getSize(), 3);

    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.getSize(), 0);
}

// Тесты для конструктора копирования
TEST(DLListTest, CopyConstructor) {
    DL_list<int> original;
    original.addHead(1);
    original.addTail(2);
    original.addTail(3);

    DL_list<int> copy(original);
    EXPECT_EQ(copy.getSize(), original.getSize());

    // проверим, что это разные объекты (можно через размер или поведение)
    original.addTail(4);
    EXPECT_EQ(original.getSize(), 4);
    EXPECT_EQ(copy.getSize(), 3);  // не изменился
}

TEST(DLListTest, CopyConstructorExceptionSafety) {
    // временно отключаем исключение при добавлении в список
    TestStruct::throw_on_copy = false;

    DL_list<TestStruct> original;
    original.addHead(TestStruct(1));
    original.addTail(TestStruct(2));
    original.addTail(TestStruct(999));  // этот элемент вызовет исключение при копировании
    original.addTail(TestStruct(4));

    // включаем исключение при копировании
    TestStruct::throw_on_copy = true;

    bool exception_thrown = false;
    try {
        DL_list<TestStruct> copy(original);
    } catch (const std::runtime_error&) {
        exception_thrown = true;
    }

    EXPECT_TRUE(exception_thrown);

    // проверяем, что исходный список не изменился
    EXPECT_EQ(original.getSize(), 4);
    EXPECT_FALSE(original.empty());

    // возвращаем флаг в исходное состояние
    TestStruct::throw_on_copy = true;
}

// тесты для оператора присваивания
TEST(DLListTest, AssignmentOperator) {
    DL_list<int> list1;
    list1.addHead(1);
    list1.addTail(2);

    DL_list<int> list2;
    list2.addHead(10);
    list2.addTail(20);
    list2.addTail(30);

    list1 = list2;

    EXPECT_EQ(list1.getSize(), list2.getSize());
    // после присваивания list1 должен быть равен list2
}

TEST(DLListTest, AssignmentSelf) {
    DL_list<int> list;
    list.addHead(1);
    list.addTail(2);
    int originalSize = list.getSize();

    list = list; // присваивание самому себе
    EXPECT_EQ(list.getSize(), originalSize);
}

// тесты для пустого списка
TEST(DLListTest, OperationsOnEmptyList) {
    DL_list<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_EQ(list.searchByValue(5), -1);
}

// тесты с использованием строки для разнообразия, а чо бы и нет
TEST(DLListTest, WithStrings) {
    DL_list<std::string> list;
    list.addHead("first");
    list.addTail("second");
    list.addAfter(0, "middle");

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.searchByValue("first"), 0);
    EXPECT_EQ(list.searchByValue("middle"), 1);
    EXPECT_EQ(list.searchByValue("second"), 2);
    EXPECT_EQ(list.searchByValue("nonexistent"), -1);

    list.removeByValue("middle");
    EXPECT_EQ(list.getSize(), 2);
}

// проверка вывода
TEST(DLListTest, PrintFunctionality) {
    DL_list<int> list;
    list.addHead(1);
    list.addTail(2);
    list.addTail(3);

    std::ostringstream oss;
    list.print(oss);
    std::string output = oss.str();

    // проверяем, что строка содержит ожидаемые элементы
    EXPECT_NE(output.find("1"), std::string::npos);
    EXPECT_NE(output.find("2"), std::string::npos);
    EXPECT_NE(output.find("3"), std::string::npos);
    EXPECT_NE(output.find("<->"), std::string::npos);

    oss.str("");  // очищаем
    oss.clear();
    list.printBackwards(oss);
    std::string outputBack = oss.str();
    EXPECT_NE(outputBack.find("1"), std::string::npos);
    EXPECT_NE(outputBack.find("2"), std::string::npos);
    EXPECT_NE(outputBack.find("3"), std::string::npos);
    EXPECT_NE(outputBack.find("<->"), std::string::npos);
}
