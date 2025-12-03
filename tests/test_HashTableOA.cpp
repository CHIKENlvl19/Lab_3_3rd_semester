// Copyright message
#include <gtest/gtest.h>
#include <string>
#include "../include/HashTableOA.hpp"

// ================== BASICS ==================

TEST(HashTableOATest, InsertAndFindIntString) {
    HashTableOA<int, std::string> ht(10);

    EXPECT_TRUE(ht.insert(5, "hello"));
    EXPECT_TRUE(ht.insert(7, "world"));

    EXPECT_EQ(ht.find(5), "hello");
    EXPECT_EQ(ht.find(7), "world");
}

TEST(HashTableOATest, InsertOverwrite) {
    HashTableOA<int, int> ht(10);

    ht.insert(3, 10);
    ht.insert(3, 20);

    EXPECT_EQ(ht.find(3), 20);
    EXPECT_EQ(ht.getSize(), 1);
}

TEST(HashTableOATest, FindMissing) {
    HashTableOA<int, int> ht(10);

    ht.insert(1, 100);

    EXPECT_EQ(ht.find(2), 0);   // default constructed int
    EXPECT_FALSE(ht.isPresent(2));
}

TEST(HashTableOATest, FindMissingAfterFullScan) {
    HashTableOA<int, int> ht(5);

    // заполняем таблицу
    for (int i = 0; i < 5; i++) {
        ht.insert(i, i * 10);
    }

    // удаляем все элементы, 5 deleted-слотов
    for (int i = 0; i < 5; i++) {
        ht.remove(i);
    }

    // теперь в таблице вообще нет fresh-слотов
    // find(999) заставит цикл пройти все i = 0..4
    int result = ht.find(999);

    EXPECT_EQ(result, 0);  // default Value()
}


TEST(HashTableOATest, RemoveKey) {
    HashTableOA<int, int> ht(10);

    ht.insert(3, 15);

    EXPECT_TRUE(ht.remove(3));
    EXPECT_FALSE(ht.isPresent(3));
    EXPECT_EQ(ht.find(3), 0);
    EXPECT_EQ(ht.getSize(), 0);
}

TEST(HashTableOATest, RemoveMissing) {
    HashTableOA<int, int> ht(10);

    ht.insert(1, 10);

    EXPECT_FALSE(ht.remove(5));
    EXPECT_EQ(ht.getSize(), 1);
}

TEST(HashTableOATest, RemoveMissingAfterFullScan) {
    HashTableOA<int, int> ht(5);

    // заполняем таблицу
    for (int i = 0; i < 5; i++) {
        ht.insert(i, i * 10);
    }

    // удаляем ВСЕ элементы, но оставляем deleted-слоты
    for (int i = 0; i < 5; i++) {
        ht.remove(i);
    }

    // теперь таблица вся в deleted-слотах → цикл пройдет полностью
    EXPECT_FALSE(ht.remove(999));  // попадает на строку 127
}


// ================== DELETED CELLS ==================

TEST(HashTableOATest, ReuseDeletedSlot) {
    HashTableOA<int, int> ht(5);

    ht.insert(1, 10);
    ht.insert(6, 20);   // collision with 1 → index + 1

    EXPECT_TRUE(ht.remove(1));

    // теперь 1-я ячейка deleted → новая вставка должна использовать её
    EXPECT_TRUE(ht.insert(11, 30));

    EXPECT_TRUE(ht.isPresent(11));
    EXPECT_EQ(ht.find(11), 30);
}

TEST(HashTableOATest, LinearProbeSkipsDeletedButKeepsSearching) {
    HashTableOA<int, int> ht(5);

    ht.insert(1, 100);
    ht.insert(6, 200);
    ht.insert(11, 300);

    ht.remove(6);

    // deleted не должен прервать поиск!
    EXPECT_EQ(ht.find(11), 300);
    EXPECT_TRUE(ht.isPresent(11));
}

TEST(HashTableOATest, IsPresentMissingAfterFullScan) {
    HashTableOA<int, int> ht(5);

    // заполняем таблицу
    for (int i = 0; i < 5; i++) {
        ht.insert(i, i * 10);
    }

    // затем удаляем всё, чтобы остались только deleted-слоты
    for (int i = 0; i < 5; i++) {
        ht.remove(i);
    }

    // теперь нет fresh-слотов
    // isPresent(999) заставит цикл пройти полностью и вернёт false на строке 86
    EXPECT_FALSE(ht.isPresent(999));
}


// ================== STRESS AND CLUSTERS ==================

TEST(HashTableOATest, ManyInsertionsWithCollisions) {
    HashTableOA<int, int> ht(50);

    for (int i = 0; i < 40; i++)
        EXPECT_TRUE(ht.insert(i * 17, i));

    for (int i = 0; i < 40; i++)
        EXPECT_TRUE(ht.isPresent(i * 17));

    for (int i = 0; i < 40; i++)
        EXPECT_EQ(ht.find(i * 17), i);
}


// ================== COPY AND ASSIGNMENT ==================

TEST(HashTableOATest, CopyConstructor) {
    HashTableOA<int, int> ht1(10);

    ht1.insert(1, 10);
    ht1.insert(2, 20);

    HashTableOA<int, int> ht2(ht1);

    EXPECT_EQ(ht2.find(1), 10);
    EXPECT_EQ(ht2.find(2), 20);
    EXPECT_TRUE(ht2.isPresent(1));
    EXPECT_TRUE(ht2.isPresent(2));

    // deep copy check
    ht1.insert(3, 30);
    EXPECT_FALSE(ht2.isPresent(3));
}

TEST(HashTableOATest, AssignmentOperator) {
    HashTableOA<int, int> ht1(10);
    HashTableOA<int, int> ht2(10);

    ht1.insert(1, 10);
    ht1.insert(2, 20);

    ht2 = ht1;

    EXPECT_EQ(ht2.find(1), 10);
    EXPECT_EQ(ht2.find(2), 20);

    ht1.insert(3, 30);
    EXPECT_FALSE(ht2.isPresent(3));
}


// ================== STRING KEYS ==================

TEST(HashTableOATest, InsertAndFindStringInt) {
    HashTableOA<std::string, int> ht(20);

    ht.insert("hello", 5);
    ht.insert("world", 10);

    EXPECT_EQ(ht.find("hello"), 5);
    EXPECT_EQ(ht.find("world"), 10);
    EXPECT_TRUE(ht.isPresent("hello"));
    EXPECT_FALSE(ht.isPresent("abc"));
}

TEST(HashTableOATest, RemoveAndReuse) {
    HashTableOA<std::string, int> ht(10);

    ht.insert("aaa", 1);
    ht.insert("bbb", 2);

    EXPECT_TRUE(ht.remove("aaa"));

    ht.insert("ccc", 3);

    EXPECT_TRUE(ht.isPresent("ccc"));
    EXPECT_EQ(ht.find("ccc"), 3);
}


// ================== EDGE CASES ==================

TEST(HashTableOATest, InsertUntilFull) {
    HashTableOA<int, int> ht(5);

    for (int i = 0; i < 5; i++)
        EXPECT_TRUE(ht.insert(i, i * 10));

    // таблица полностью заполнена
    testing::internal::CaptureStderr();
    EXPECT_FALSE(ht.insert(100, 200));
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_NE(output.find("Error: table is full"), std::string::npos);
}

TEST(HashTableOATest, LoadFactorCorrect) {
    HashTableOA<int, int> ht(10);

    EXPECT_FLOAT_EQ(ht.getLoadFactor(), 0.0f);

    ht.insert(1, 10);
    ht.insert(2, 10);
    ht.insert(3, 10);

    EXPECT_FLOAT_EQ(ht.getLoadFactor(), 0.3f);

    ht.remove(2);

    EXPECT_FLOAT_EQ(ht.getLoadFactor(), 0.2f);
}

TEST(HashTableOATest, getLoadFactorReturnsZero) {
    HashTableOA<int, int> ht(0);

    EXPECT_FLOAT_EQ(ht.getLoadFactor(), 0.0f);
}
