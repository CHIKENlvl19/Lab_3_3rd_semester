// Copyright message
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../include/myQueue.hpp"

// вспомогательная функция: читает все строки файла в вектор
std::vector<std::string> readFile(const std::string& filename) {
    std::ifstream in(filename);
    std::vector<std::string> out;
    std::string line;
    while (std::getline(in, line)) out.push_back(line);
    return out;
}

// вспомогательная функция: захват std::cout
template <typename T>
std::string capturePrint(const myQueue<T>& q) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    q.print();

    std::cout.rdbuf(old);
    return buffer.str();
}

// конструкторы + очистка

TEST(QueueTest, DefaultInitialization) {
    myQueue<int> q;
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, NegativeCapacityDefaultsToFour) {
    myQueue<int> q(-10);
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, CleanEmptiesQueue) {
    myQueue<int> q;
    q.push(1, "Q_test.txt");
    q.clean();
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, CopyingConstructorWorks) {
    std::ofstream("Q_test.txt", std::ios::trunc).close();

    myQueue<int> q;
    q.push(1, "Q_test.txt");
    q.push(2, "Q_test.txt");

    myQueue<int> q2(q);
    EXPECT_EQ(q2.getSize(), 2);
    EXPECT_EQ(q2.getCapacity(), 4);
}

// вставка / удаление + расширение вместимости

TEST(QueueTest, PushIncreasesSize) {
    std::string fname = "Q_push.txt";
    std::ofstream(fname).close();  // очищаем файл

    myQueue<std::string> q;
    q.push("A", fname);
    q.push("B", fname);

    EXPECT_EQ(q.getSize(), 2);

    auto lines = readFile(fname);
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "A");
    EXPECT_EQ(lines[1], "B");
}

TEST(QueueTest, PopRemovesFrontElement) {
    std::string fname = "Q_pop.txt";

    std::ofstream out(fname);
    out << "X\nY\nZ\n";
    out.close();

    myQueue<std::string> q;
    q.pop(fname);

    auto lines = readFile(fname);
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "Y");
    EXPECT_EQ(lines[1], "Z");
}

TEST(QueueTest, PopFromEmptyThrows) {
    std::string fname = "Q_empty.txt";
    std::ofstream(fname).close();

    myQueue<int> q;
    EXPECT_THROW(q.pop(fname), std::underflow_error);
}


// проверка увеличения вместимости

TEST(QueueTest, PushTriggersCapacityIncrease) {
    std::string fname = "Q_grow.txt";
    std::ofstream(fname).close();

    myQueue<std::string> q(2);
    q.push("A", fname);
    q.push("B", fname);
    q.push("C", fname);  // должен увеличить capacity

    EXPECT_EQ(q.getSize(), 3);

    auto lines = readFile(fname);
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[2], "C");  // новый элемент
}

TEST(QueueTest, PushTriggersIncreaseDefaultCapacity) {
    std::string fname = "Q_cap.txt";
    std::ofstream(fname).close();

    myQueue<int> q;
    q.push(1, fname);
    q.push(2, fname);
    q.push(3, fname);
    q.push(4, fname);
    q.push(5, fname);

    EXPECT_EQ(q.getSize(), 5);
    EXPECT_EQ(q.getCapacity(), 8);
}

// загрузка из файла + исключения

TEST(QueueTest, LoadFromNonexistentFileCreatesEmptyQueue) {
    std::string fname = "NO_SUCH_FILE.txt";

    remove(fname.c_str());

    myQueue<std::string> q;
    q.loadFromFile(fname);

    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, LoadFromFileRestoresElements) {
    std::string fname = "Q_load.txt";

    std::ofstream out(fname);
    out << "One\nTwo\nThree\n";
    out.close();

    myQueue<std::string> q;
    q.loadFromFile(fname);

    EXPECT_EQ(q.getSize(), 3);

    auto lines = readFile(fname);
    EXPECT_EQ(lines.size(), 3);
}

TEST(QueueTest, LoadFromFileExceptionWorks) {
    myQueue<int> q;
    q.push(1, "Q_throws.txt");

    EXPECT_THROW(q.saveToFile("/"), std::runtime_error);
}

TEST(QueueTest, LoadFromFileIncreasesCapacity) {
    std::ofstream file("test_capacity.txt");
    for (int i = 0; i < 10; ++i) {
        file << i << "\n";
    }
    file.close();

    myQueue<int> q;
    q.loadFromFile("test_capacity.txt");

    EXPECT_GE(q.getCapacity(), 10);  // capacity должно быть >= 10
    EXPECT_EQ(q.getSize(), 10);
}


// сохранение в файл

TEST(QueueTest, SaveToFileWritesCorrectOrder) {
    std::string fname = "Q_save.txt";
    std::ofstream(fname).close();

    myQueue<int> q;
    q.push(1, fname);
    q.push(2, fname);
    q.push(3, fname);

    auto lines = readFile(fname);
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "1");
    EXPECT_EQ(lines[1], "2");
    EXPECT_EQ(lines[2], "3");
}


// печать

TEST(QueueTest, PrintShowsCorrectOutput) {
    std::string fname = "Q_print.txt";
    std::ofstream(fname).close();

    myQueue<std::string> q;
    q.push("Hello", fname);
    q.push("World", fname);

    std::string output = capturePrint(q);
    EXPECT_NE(output.find("Hello"), std::string::npos);
    EXPECT_NE(output.find("World"), std::string::npos);
}

TEST(QueueTest, PrintEmptyQueue) {
    myQueue<int> q;
    std::string output = capturePrint(q);

    EXPECT_NE(output.find("Queue is empty!"), std::string::npos);
}
