// Copyright
#include <iostream>
#include <cassert>
#include "include/myArray.hpp"
#include "include/myQueue.hpp"
#include "include/myStack.hpp"
#include "include/SL_List.hpp"
#include "include/DL_List.hpp"
#include "include/CBT.hpp"
#include "include/HashTableOA.hpp"
#include "include/HashTableChains.hpp"

#define TEST(name) std::cout << "Testing " << name << "... "; std::cout.flush();
#define PASS() std::cout << "OK\n";
#define FAIL(msg) do { std::cout << "FAILED: " << msg << "\n"; return false; } while(0)

bool test_array_serialization() {
    TEST("myArray");

    // текстовая сериализация
    {
        myArray<int> arr;
        arr.push_back(10);
        arr.push_back(20);
        arr.push_back(30);
        arr.push_back(40);
        arr.push_back(50);

        arr.saveText("test_array.txt");

        myArray<int> loaded;
        loaded.loadText("test_array.txt");

        if (loaded.getSize() != 5) FAIL("Text: size mismatch");
        if (loaded[0] != 10 || loaded[4] != 50) FAIL("Text: data mismatch");
    }

    // бинарная сериализация
    {
        myArray<int> arr;
        arr.push_back(100);
        arr.push_back(200);
        arr.push_back(300);

        arr.saveBinary("test_array.bin");

        myArray<int> loaded;
        loaded.loadBinary("test_array.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
        if (loaded[0] != 100 || loaded[2] != 300) FAIL("Binary: data mismatch");
    }

    PASS();
    return true;
}

bool test_queue_serialization() {
    TEST("myQueue");

    // текстовая сериализация
    {
        myQueue<int> q;
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);

        q.saveText("test_queue.txt");

        myQueue<int> loaded;
        loaded.loadText("test_queue.txt");

        if (loaded.getSize() != 4) FAIL("Text: size mismatch");
    }

    // бинарная сериализация
    {
        myQueue<int> q;
        q.push(10);
        q.push(20);
        q.push(30);

        q.saveBinary("test_queue.bin");

        myQueue<int> loaded;
        loaded.loadBinary("test_queue.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
    }

    PASS();
    return true;
}

bool test_stack_serialization() {
    TEST("Stack");

    // текстовая сериализация
    {
        Stack<int> s;
        s.push(5);
        s.push(10);
        s.push(15);

        s.saveText("test_stack.txt");

        Stack<int> loaded;
        loaded.loadText("test_stack.txt");

        if (loaded.getSize() != 3) FAIL("Text: size mismatch");
        if (loaded.peek() != 15) FAIL("Text: top element mismatch");
    }

    // бинарная сериализация
    {
        Stack<int> s;
        s.push(100);
        s.push(200);
        s.push(300);
        s.push(400);

        s.saveBinary("test_stack.bin");

        Stack<int> loaded;
        loaded.loadBinary("test_stack.bin");

        if (loaded.getSize() != 4) FAIL("Binary: size mismatch");
        if (loaded.peek() != 400) FAIL("Binary: top element mismatch");
    }

    PASS();
    return true;
}

bool test_sll_serialization() {
    TEST("SinglyLinkedList");

    // текстовая сериализация
    {
        SinglyLinkedList<int> list;
        list.pushBack(1);
        list.pushBack(2);
        list.pushBack(3);
        list.pushBack(4);

        list.saveText("test_sll.txt");

        SinglyLinkedList<int> loaded;
        loaded.loadText("test_sll.txt");

        if (loaded.getSize() != 4) FAIL("Text: size mismatch");
        if (loaded.front() != 1) FAIL("Text: front mismatch");
    }

    // бинарная сериализация
    {
        SinglyLinkedList<int> list;
        list.pushBack(10);
        list.pushBack(20);
        list.pushBack(30);

        list.saveBinary("test_sll.bin");

        SinglyLinkedList<int> loaded;
        loaded.loadBinary("test_sll.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
        if (loaded.front() != 10) FAIL("Binary: front mismatch");
    }

    PASS();
    return true;
}

bool test_dll_serialization() {
    TEST("DL_list");

    // текстовая сериализация
    {
        DL_list<int> list;
        list.addTail(5);
        list.addTail(10);
        list.addTail(15);
        list.addTail(20);

        list.saveText("test_dll.txt");

        DL_list<int> loaded;
        loaded.loadText("test_dll.txt");

        if (loaded.getSize() != 4) FAIL("Text: size mismatch");
        if (loaded.getHead()->value != 5) FAIL("Text: head mismatch");
    }

    // бинарная сериализация
    {
        DL_list<int> list;
        list.addTail(100);
        list.addTail(200);
        list.addTail(300);

        list.saveBinary("test_dll.bin");

        DL_list<int> loaded;
        loaded.loadBinary("test_dll.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
        if (loaded.getHead()->value != 100) FAIL("Binary: head mismatch");
    }

    PASS();
    return true;
}

bool test_cbt_serialization() {
    TEST("CompleteBinaryTree");

    // текстовая сериализация
    {
        CompleteBinaryTree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(3);
        tree.insert(4);
        tree.insert(5);

        tree.saveText("test_cbt.txt");

        CompleteBinaryTree<int> loaded;
        loaded.loadText("test_cbt.txt");

        if (loaded.getSize() != 5) FAIL("Text: size mismatch");
        if (loaded.getRoot() != 1) FAIL("Text: root mismatch");
    }

    // бинарная сериализация
    {
        CompleteBinaryTree<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);

        tree.saveBinary("test_cbt.bin");

        CompleteBinaryTree<int> loaded;
        loaded.loadBinary("test_cbt.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
        if (loaded.getRoot() != 10) FAIL("Binary: root mismatch");
    }

    PASS();
    return true;
}

bool test_hashtable_oa_serialization() {
    TEST("HashTableOA");

    // текстовая сериализация
    {
        HashTableOA<int, int> table(100);
        table.insert(1, 10);
        table.insert(2, 20);
        table.insert(3, 30);
        table.insert(4, 40);

        table.saveText("test_htoa.txt");

        HashTableOA<int, int> loaded(100);
        loaded.loadText("test_htoa.txt");

        if (loaded.getSize() != 4) FAIL("Text: size mismatch");
        if (loaded.find(1) != 10 || loaded.find(4) != 40) FAIL("Text: data mismatch");
    }

    // бинарная сериализация
    {
        HashTableOA<int, int> table(100);
        table.insert(5, 50);
        table.insert(6, 60);
        table.insert(7, 70);

        table.saveBinary("test_htoa.bin");

        HashTableOA<int, int> loaded(100);
        loaded.loadBinary("test_htoa.bin");

        if (loaded.getSize() != 3) FAIL("Binary: size mismatch");
        if (loaded.find(5) != 50 || loaded.find(7) != 70) FAIL("Binary: data mismatch");
    }

    PASS();
    return true;
}

bool test_hashtable_chains_serialization() {
    TEST("HashTableChains");

    // текстовая сериализация
    {
        HashTable<int, int> table(100);
        table.insert(1, 100);
        table.insert(2, 200);
        table.insert(3, 300);

        table.saveText("test_htc.txt");

        HashTable<int, int> loaded(100);
        loaded.loadText("test_htc.txt");
    
        if (!loaded.isPresent(1) || !loaded.isPresent(3)) FAIL("Text: keys missing");
        if (loaded.find(1) != 100 || loaded.find(3) != 300) FAIL("Text: data mismatch");
    }

    // бинарная сериализация
    {
        HashTable<int, int> table(100);
        table.insert(10, 1000);
        table.insert(20, 2000);

        table.saveBinary("test_htc.bin");

        HashTable<int, int> loaded(100);
        loaded.loadBinary("test_htc.bin");

        if (!loaded.isPresent(10) || !loaded.isPresent(20)) FAIL("Binary: keys missing");
        if (loaded.find(10) != 1000 || loaded.find(20) != 2000) FAIL("Binary: data mismatch");
    }

    PASS();
    return true;
}

int main() {
    std::cout << "=== Testing Serialization/Deserialization ===\n\n";

    int passed = 0;
    int total = 0;

    total++; if (test_array_serialization()) passed++;
    total++; if (test_queue_serialization()) passed++;
    total++; if (test_stack_serialization()) passed++;
    total++; if (test_sll_serialization()) passed++;
    total++; if (test_dll_serialization()) passed++;
    total++; if (test_cbt_serialization()) passed++;
    total++; if (test_hashtable_oa_serialization()) passed++;
    total++; if (test_hashtable_chains_serialization()) passed++;

    std::cout << "\n=== Results: " << passed << "/" << total << " tests passed ===\n";

    return (passed == total) ? 0 : 1;
}
