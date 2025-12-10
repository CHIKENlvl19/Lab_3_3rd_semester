// Copyright
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "../include/myQueue.hpp"

// Вспомогательная функция: захват std::cout для любого типа
template <typename T>
std::string capturePrint(const myQueue<T>& q) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    return buffer.str();
}


// КОНСТРУКТОРЫ + ОЧИСТКА
TEST(QueueTest, DefaultInitialization) {
    myQueue<int> q;
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, NegativeCapacityDefaultsToFour) {
    myQueue<int> q(-10);
    EXPECT_EQ(q.getSize(), 0);
    EXPECT_EQ(q.getCapacity(), 4);
}

TEST(QueueTest, CapacityInitialization) {
    myQueue<int> q(8);
    EXPECT_EQ(q.getSize(), 0);
    EXPECT_EQ(q.getCapacity(), 8);
}

TEST(QueueTest, CleanEmptiesQueue) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.clean();
    EXPECT_EQ(q.getSize(), 0);
}

TEST(QueueTest, CopyingConstructorWorks) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    myQueue<int> q2(q);
    EXPECT_EQ(q2.getSize(), 2);
    EXPECT_EQ(q2.getCapacity(), 4);
}

TEST(QueueTest, AssignmentOperatorWorks) {
    myQueue<int> q1;
    q1.push(10);
    q1.push(20);

    myQueue<int> q2;
    q2 = q1;

    EXPECT_EQ(q2.getSize(), 2);
    EXPECT_EQ(q2.getCapacity(), q1.getCapacity());
}

// PUSH И SIZE
TEST(QueueTest, PushSingleElement) {
    myQueue<int> q;
    q.push(10);
    EXPECT_EQ(q.getSize(), 1);
}

TEST(QueueTest, PushMultipleElements) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    EXPECT_EQ(q.getSize(), 3);
}

TEST(QueueTest, PushIncreasesSize) {
    myQueue<int> q;
    for (int i = 0; i < 5; i++) {
        q.push(i);
        EXPECT_EQ(q.getSize(), i + 1);
    }
}

// РАСШИРЕНИЕ ВМЕСТИМОСТИ
TEST(QueueTest, PushTriggersCapacityIncrease) {
    myQueue<int> q(2);
    q.push(1);
    q.push(2);
    int oldCapacity = q.getCapacity();
    q.push(3);  // должен увеличить capacity
    EXPECT_EQ(q.getSize(), 3);
    EXPECT_GT(q.getCapacity(), oldCapacity);
}

TEST(QueueTest, PushTriggersIncreaseDefaultCapacity) {
    myQueue<int> q;
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }
    EXPECT_EQ(q.getSize(), 5);
    EXPECT_GE(q.getCapacity(), 5);
}

TEST(QueueTest, CapacityDoublesWhenExceeded) {
    myQueue<int> q(4);
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }
    EXPECT_GE(q.getCapacity(), 8);
}

TEST(QueueTest, MultipleCapacityIncreases) {
    myQueue<int> q(2);
    for (int i = 0; i < 20; i++) {
        q.push(i);
    }
    EXPECT_EQ(q.getSize(), 20);
    EXPECT_GE(q.getCapacity(), 20);
}

// POP
TEST(QueueTest, PopRemovesFrontElement) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    EXPECT_EQ(q.getSize(), 2);
}

TEST(QueueTest, PopMultipleElements) {
    myQueue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    q.pop();
    q.pop();
    EXPECT_EQ(q.getSize(), 1);
}

TEST(QueueTest, PopFromEmptyThrows) {
    myQueue<int> q;
    EXPECT_THROW(q.pop(), std::underflow_error);
}

TEST(QueueTest, PopUntilEmpty) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    q.pop();
    EXPECT_EQ(q.getSize(), 1);
    q.pop();
    EXPECT_EQ(q.getSize(), 0);
}

// ПЕЧАТЬ
TEST(QueueTest, PrintShowsCorrectOutput) {
    myQueue<int> q;
    q.push(10);
    q.push(20);
    std::string output = capturePrint(q);
    EXPECT_NE(output.find("10"), std::string::npos);
    EXPECT_NE(output.find("20"), std::string::npos);
}

TEST(QueueTest, PrintEmptyQueue) {
    myQueue<int> q;
    std::string output = capturePrint(q);
    EXPECT_NE(output.find("empty"), std::string::npos);
}

TEST(QueueTest, PrintAfterOperations) {
    myQueue<int> q;
    q.push(5);
    q.push(15);
    q.pop();
    std::string output = capturePrint(q);
    EXPECT_NE(output.find("15"), std::string::npos);
}

// ФАЙЛОВЫЕ ОПЕРАЦИИ - TEXT
TEST(QueueTest, SaveLoadTextWorks) {
    myQueue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    std::string fname = "test_queue_int.txt";
    q.saveText(fname);

    myQueue<int> q2;
    q2.loadText(fname);

    EXPECT_EQ(q2.getSize(), q.getSize());
    EXPECT_EQ(q2.getCapacity(), q.getCapacity());

    remove(fname.c_str());
}

TEST(QueueTest, SaveTextCreatesTempFile) {
    myQueue<int> q;
    for (int i = 0; i < 5; i++) {
        q.push(i);
    }

    std::string fname = "test_queue_save.txt";
    q.saveText(fname);

    std::ifstream file(fname);
    EXPECT_TRUE(file.is_open());
    file.close();

    remove(fname.c_str());
}

TEST(QueueTest, SaveTextWithInvalidPath) {
    myQueue<int> q;
    q.push(1);

    EXPECT_THROW(q.saveText("/invalid/path/queue.txt"), std::runtime_error);
}

TEST(QueueTest, LoadTextFromNonexistentFile) {
    std::string fname = "nonexistent_queue_file.txt";
    remove(fname.c_str());

    myQueue<int> q;
    EXPECT_THROW(q.loadText(fname), std::runtime_error);
}

TEST(QueueTest, LoadTextIncreasesCapacity) {
    std::string fname = "test_queue_capacity.txt";
    std::ofstream file(fname);
    file << "10 20\n";  // size=10, capacity=20
    for (int i = 0; i < 10; ++i) {
        file << i << " ";
    }
    file.close();

    myQueue<int> q;
    q.loadText(fname);

    EXPECT_EQ(q.getSize(), 10);
    EXPECT_EQ(q.getCapacity(), 20);

    remove(fname.c_str());
}

// ФАЙЛОВЫЕ ОПЕРАЦИИ - BINARY
TEST(QueueTest, SaveLoadBinaryWorks) {
    myQueue<int> q;
    q.push(100);
    q.push(200);
    q.push(300);

    std::string fname = "test_queue_binary.bin";
    q.saveBinary(fname);

    myQueue<int> q2;
    q2.loadBinary(fname);

    EXPECT_EQ(q2.getSize(), q.getSize());
    EXPECT_EQ(q2.getCapacity(), q.getCapacity());

    remove(fname.c_str());
}

TEST(QueueTest, SaveBinaryCreatesTempFile) {
    myQueue<int> q;
    for (int i = 0; i < 5; i++) {
        q.push(i * 10);
    }

    std::string fname = "test_queue_save_bin.bin";
    q.saveBinary(fname);

    std::ifstream file(fname, std::ios::binary);
    EXPECT_TRUE(file.is_open());
    file.close();

    remove(fname.c_str());
}

TEST(QueueTest, SaveBinaryWithInvalidPath) {
    myQueue<int> q;
    q.push(1);

    EXPECT_THROW(q.saveBinary("/invalid/path/queue.bin"), std::runtime_error);
}

TEST(QueueTest, LoadBinaryFromNonexistentFile) {
    std::string fname = "nonexistent_queue_file.bin";
    remove(fname.c_str());

    myQueue<int> q;
    EXPECT_THROW(q.loadBinary(fname), std::runtime_error);
}

// СТРЕСС-ТЕСТЫ
TEST(QueueTest, LargeNumberOfOperations) {
    myQueue<int> q;

    // Добавляем много элементов
    for (int i = 0; i < 1000; i++) {
        q.push(i);
    }
    EXPECT_EQ(q.getSize(), 1000);

    // Удаляем половину
    for (int i = 0; i < 500; i++) {
        q.pop();
    }
    EXPECT_EQ(q.getSize(), 500);
}

TEST(QueueTest, AlternatingPushAndPop) {
    myQueue<int> q;
    for (int i = 0; i < 100; i++) {
        q.push(i);
        if (i % 2 == 0 && i > 0) {
            q.pop();
        }
    }
    EXPECT_GT(q.getSize(), 0);
}

TEST(QueueTest, CircularBufferWraparound) {
    myQueue<int> q(4);

    // Заполняем
    for (int i = 0; i < 4; i++) {
        q.push(i);
    }

    // Удаляем и добавляем снова (wraparound)
    q.pop();
    q.pop();
    q.push(100);
    q.push(200);

    EXPECT_EQ(q.getSize(), 4);
}

// ТЕСТЫ С STRING

TEST(QueueTestString, PushAndPop) {
    myQueue<std::string> q;
    q.push("Hello");
    q.push("World");
    EXPECT_EQ(q.getSize(), 2);

    q.pop();
    EXPECT_EQ(q.getSize(), 1);
}

TEST(QueueTestString, SaveLoadText) {
    myQueue<std::string> q;
    q.push("First");
    q.push("Second");
    q.push("Third");

    std::string fname = "test_queue_string.txt";
    q.saveText(fname);

    myQueue<std::string> q2;
    q2.loadText(fname);

    EXPECT_EQ(q2.getSize(), 3);

    remove(fname.c_str());
}

TEST(QueueTestString, PrintOutput) {
    myQueue<std::string> q;
    q.push("Alpha");
    q.push("Beta");

    std::string output = capturePrint(q);
    EXPECT_NE(output.find("Alpha"), std::string::npos);
    EXPECT_NE(output.find("Beta"), std::string::npos);
}

TEST(QueueTestString, CapacityIncrease) {
    myQueue<std::string> q(2);
    q.push("A");
    q.push("B");
    q.push("C");  // должен увеличить capacity

    EXPECT_EQ(q.getSize(), 3);
    EXPECT_GE(q.getCapacity(), 3);
}

// ТЕСТЫ С DOUBLE

TEST(QueueTestDouble, BasicOperations) {
    myQueue<double> q;
    q.push(3.14);
    q.push(2.71);
    q.push(1.41);

    EXPECT_EQ(q.getSize(), 3);

    q.pop();
    EXPECT_EQ(q.getSize(), 2);
}

TEST(QueueTestDouble, SaveLoadBinary) {
    myQueue<double> q;
    q.push(1.1);
    q.push(2.2);
    q.push(3.3);

    std::string fname = "test_queue_double.bin";
    q.saveBinary(fname);

    myQueue<double> q2;
    q2.loadBinary(fname);

    EXPECT_EQ(q2.getSize(), 3);

    remove(fname.c_str());
}
