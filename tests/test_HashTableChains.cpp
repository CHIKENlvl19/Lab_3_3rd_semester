// Copyright message
#include <gtest/gtest.h>
#include <gmpxx.h>
#include <sstream>
#include "../include/HashTableChains.hpp"

// Используем int для ключа и значения, т.к. int конвертируется в mpz_class.
using IntHashTable = HashTable<int, int>;

// 1. Тест инициализации и емкости
TEST(HashTableTest, InitializationAndCapacity) {
    IntHashTable ht_default;
    // Capacity 5 по умолчанию, но мы не можем его проверить напрямую.
    EXPECT_FALSE(ht_default.isPresent(1));

    IntHashTable ht_cap(15);
    EXPECT_FALSE(ht_cap.isPresent(100));
}

// 2. Тест вставки и поиска (Insert & Find)
TEST(HashTableTest, InsertAndFind) {
    IntHashTable ht(10);

    ht.insert(1, 100);
    ht.insert(5, 500);
    ht.insert(10, 1000);  // Ключ 10

    EXPECT_TRUE(ht.isPresent(1));
    EXPECT_TRUE(ht.isPresent(5));
    EXPECT_TRUE(ht.isPresent(10));
    EXPECT_FALSE(ht.isPresent(99));

    EXPECT_EQ(ht.find(1), 100);
    EXPECT_EQ(ht.find(5), 500);
    EXPECT_EQ(ht.find(10), 1000);
    EXPECT_EQ(ht.find(99), 0);  // Find должен вернуть Value() (0 для int)
}

// 3. Тест дубликатов
// Логика: если ключ есть, он НЕ обновляется, а выводится ошибка в cerr.
TEST(HashTableTest, InsertDuplicate) {
    IntHashTable ht(5);

    ht.insert(10, 500);
    ht.insert(10, 999);

    EXPECT_EQ(ht.find(10), 500);  // Значение должно остаться старым
}

// 4. Тест удаления (Remove)
TEST(HashTableTest, Remove) {
    IntHashTable ht(5);

    ht.insert(1, 10);
    ht.insert(2, 20);

    // Удаление существующего
    ht.remove(1);

    EXPECT_FALSE(ht.isPresent(1));
    EXPECT_TRUE(ht.isPresent(2));

    // Повторное удаление (должен быть вывод ошибки, но не краш)
    EXPECT_NO_THROW(ht.remove(1));

    // Удаление несуществующего ключа
    EXPECT_NO_THROW(ht.remove(999));
}

// 5. Тест коллизий (Collision Handling)
// Вставляем больше элементов, чем емкость, чтобы гарантировать коллизии
TEST(HashTableTest, CollisionHandling) {
    int capacity = 5;
    IntHashTable ht(capacity);

    // Вставляем 10 элементов
    for (int i = 0; i < 10; ++i) {
        ht.insert(i, i * 100);
    }

    // Проверяем, что ВСЕ элементы сохранены (метод цепочек должен справиться)
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(ht.isPresent(i)) << "Key " << i << " was lost due to collision";
        EXPECT_EQ(ht.find(i), i * 100);
    }
}

// 6. Тест очистки (Clean)
TEST(HashTableTest, Clean) {
    IntHashTable ht(10);
    ht.insert(1, 1);
    ht.insert(2, 2);

    ht.clean();

    EXPECT_FALSE(ht.isPresent(1));
    // При size=0, capacity остается, но buckets = nullptr.
}

// 7. Тест "Правила трёх" (Memory Safety / Deep Copy)
TEST(HashTableTest, DeepCopyAndAssignment) {
    IntHashTable h1(5);
    h1.insert(10, 100);

    // 7.1 Конструктор копирования
    IntHashTable h2 = h1;

    // Изменяем оригинал (h1)
    h1.insert(20, 200);
    h1.remove(10);

    // Проверяем, что h2 осталась старой (глубокая копия)
    EXPECT_TRUE(h2.isPresent(10));
    EXPECT_FALSE(h2.isPresent(20));
    EXPECT_TRUE(h1.isPresent(20));
    EXPECT_FALSE(h1.isPresent(10));

    // 7.2 Оператор присваивания
    IntHashTable h3(5);
    h3.insert(30, 300);  // Старое содержимое

    h3 = h2;  // Присваиваем h2 (содержит только 10:100)

    // Проверяем, что h3 получила содержимое h2 и независима
    EXPECT_TRUE(h3.isPresent(10));
    EXPECT_FALSE(h3.isPresent(30));

    // Меняем h2
    h2.clean();  // h2 теперь пуста

    // h3 должна остаться нетронутой
    EXPECT_TRUE(h3.isPresent(10));
}
