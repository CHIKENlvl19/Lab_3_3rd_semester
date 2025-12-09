package datastructures

import (
	"os"
	"testing"
)

func TestHashTableChainsNew(t *testing.T) {
	ht := NewHashTableChains(16)
	if ht.Size() != 0 {
		t.Errorf("expected size 0, got %d", ht.Size())
	}
	if ht.Capacity() != 16 {
		t.Errorf("expected capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableChainsInsert(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)
	ht.Insert(3, 300)

	if ht.Size() != 3 {
		t.Errorf("expected size 3, got %d", ht.Size())
	}
}

func TestHashTableChainsGet(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)

	val, err := ht.Get(1)
	if err != nil {
		t.Fatalf("Get failed: %v", err)
	}
	if val != 100 {
		t.Errorf("expected value 100, got %d", val)
	}

	_, err = ht.Get(999)
	if err == nil {
		t.Error("expected error for non-existent key")
	}
}

func TestHashTableChainsUpdate(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(1, 100)
	ht.Insert(1, 200)

	val, _ := ht.Get(1)
	if val != 200 {
		t.Errorf("expected updated value 200, got %d", val)
	}

	if ht.Size() != 1 {
		t.Errorf("expected size 1 after update, got %d", ht.Size())
	}
}

func TestHashTableChainsRemove(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)

	err := ht.Remove(1)
	if err != nil {
		t.Fatalf("Remove failed: %v", err)
	}

	if ht.Size() != 1 {
		t.Errorf("expected size 1, got %d", ht.Size())
	}

	if ht.Contains(1) {
		t.Error("key 1 should not exist after removal")
	}
}

func TestHashTableChainsCollisions(t *testing.T) {
	ht := NewHashTableChains(4) // Маленький capacity для коллизий

	// Вставляем элементы, которые могут вызвать коллизии
	for i := 0; i < 20; i++ {
		ht.Insert(i, i*10)
	}

	if ht.Size() != 20 {
		t.Errorf("expected size 20, got %d", ht.Size())
	}

	// Проверяем, что все элементы доступны
	for i := 0; i < 20; i++ {
		val, err := ht.Get(i)
		if err != nil {
			t.Errorf("failed to get key %d: %v", i, err)
		}
		if val != i*10 {
			t.Errorf("value mismatch for key %d: expected %d, got %d", i, i*10, val)
		}
	}
}

func TestHashTableChainsResize(t *testing.T) {
	ht := NewHashTableChains(4)
	for i := 0; i < 10; i++ {
		ht.Insert(i, i*10)
	}

	if ht.Capacity() <= 4 {
		t.Error("expected capacity to increase after multiple insertions")
	}

	for i := 0; i < 10; i++ {
		val, err := ht.Get(i)
		if err != nil {
			t.Errorf("failed to get key %d after resize", i)
		}
		if val != i*10 {
			t.Errorf("value mismatch for key %d: expected %d, got %d", i, i*10, val)
		}
	}
}

func TestHashTableChainsSaveLoadText(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)
	ht.Insert(3, 300)

	filename := "test_ht_chains.txt"
	defer os.Remove(filename)

	if err := ht.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	ht2 := NewHashTableChains(16)
	if err := ht2.LoadText(filename); err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if ht2.Size() != ht.Size() {
		t.Errorf("size mismatch: expected %d, got %d", ht.Size(), ht2.Size())
	}

	val, _ := ht2.Get(2)
	if val != 200 {
		t.Errorf("value mismatch: expected 200, got %d", val)
	}
}

func TestHashTableChainsSaveLoadBinary(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 1000)
	ht.Insert(20, 2000)
	ht.Insert(30, 3000)

	filename := "test_ht_chains.bin"
	defer os.Remove(filename)

	if err := ht.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	ht2 := NewHashTableChains(16)
	if err := ht2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if ht2.Size() != ht.Size() {
		t.Errorf("size mismatch: expected %d, got %d", ht.Size(), ht2.Size())
	}
}

func TestHashTableChainsNewZeroCapacity(t *testing.T) {
	ht := NewHashTableChains(0)
	if ht.Capacity() != 16 {
		t.Errorf("Expected default capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableChainsNewNegativeCapacity(t *testing.T) {
	ht := NewHashTableChains(-5)
	if ht.Capacity() != 16 {
		t.Errorf("Expected default capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableChainsInsertUpdate(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	ht.Insert(10, 200) // Update existing key
	
	val, err := ht.Get(10)
	if err != nil {
		t.Fatalf("Get failed: %v", err)
	}
	if val != 200 {
		t.Errorf("Expected updated value 200, got %d", val)
	}
	
	// Size should not increase on update
	if ht.Size() != 1 {
		t.Errorf("Expected size 1, got %d", ht.Size())
	}
}

func TestHashTableChainsGetNotFound(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	
	_, err := ht.Get(999)
	if err == nil {
		t.Error("Expected error for non-existent key")
	}
}

func TestHashTableChainsRemoveNotFound(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	
	err := ht.Remove(999)
	if err == nil {
		t.Error("Expected error when removing non-existent key")
	}
}

func TestHashTableChainsContainsTrue(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	
	if !ht.Contains(10) {
		t.Error("Expected Contains(10) to be true")
	}
}

func TestHashTableChainsContainsFalse(t *testing.T) {
	ht := NewHashTableChains(16)
	
	if ht.Contains(10) {
		t.Error("Expected Contains(10) to be false")
	}
}

func TestHashTableChainsClear(t *testing.T) {
	ht := NewHashTableChains(16)
	for i := 0; i < 10; i++ {
		ht.Insert(i, i*10)
	}
	
	ht.Clear()
	if ht.Size() != 0 {
		t.Errorf("Expected size 0 after clear, got %d", ht.Size())
	}
	if ht.LoadFactor() != 0.0 {
		t.Errorf("Expected load factor 0.0 after clear, got %f", ht.LoadFactor())
	}
}

func TestHashTableChainsPrint(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	ht.Insert(20, 200)
	ht.Print()
}

func TestHashTableChainsSaveTextError(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	
	err := ht.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestHashTableChainsLoadTextError(t *testing.T) {
	ht := NewHashTableChains(16)
	err := ht.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestHashTableChainsSaveBinaryError(t *testing.T) {
	ht := NewHashTableChains(16)
	ht.Insert(10, 100)
	
	err := ht.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestHashTableChainsLoadBinaryError(t *testing.T) {
	ht := NewHashTableChains(16)
	err := ht.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestHashTableChainsLoadTextInvalidFormat1(t *testing.T) {
	// файл с некорректным первым заголовком
	filename := "test_invalid1.txt"
	file, _ := os.Create(filename)
	file.WriteString("invalid header\n") // без 5 чисел
	file.Close()
	defer os.Remove(filename)
	
	ht := NewHashTableChains(16)
	err := ht.LoadText(filename)
	if err == nil {
		t.Error("Expected error for invalid file format")
	}
}

func TestHashTableChainsLoadTextInvalidFormat2(t *testing.T) {
	// файл с некорректными данными
	filename := "test_invalid2.txt"
	file, _ := os.Create(filename)
	file.WriteString("5 16 1 0 1000000007\n") // корректный заголовок
	file.WriteString("10\n") // некорректная пара (только одно число)
	file.Close()
	defer os.Remove(filename)
	
	ht := NewHashTableChains(16)
	err := ht.LoadText(filename)
	// не должно быть фатальной ошибки, просто пропустит некорректную строку
	if err != nil {
		t.Errorf("LoadText failed: %v", err)
	}
}

func TestHashTableChainsRemoveFromChain(t *testing.T) {
	ht := NewHashTableChains(2) // маленькая таблица для коллизий
	
	// вставляем элементы которые попадут в одну цепочку
	ht.Insert(0, 10)
	ht.Insert(2, 20)  // скорее всего коллизия с 0
	ht.Insert(4, 30)  // и это тоже коллизия ХD
	
	// удаляем элемент из середины цепочки
	err := ht.Remove(2)
	if err != nil {
		t.Errorf("Remove from chain failed: %v", err)
	}
	
	// проверяем что другие элементы на месте
	_, err1 := ht.Get(0)
	_, err2 := ht.Get(4)
	if err1 != nil || err2 != nil {
		t.Error("Other elements in chain should still exist")
	}
}
