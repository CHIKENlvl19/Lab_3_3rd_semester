package datastructures

import (
	"os"
	"testing"
)

func TestHashTableOANew(t *testing.T) {
	ht := NewHashTableOA(16)
	if ht.Size() != 0 {
		t.Errorf("expected size 0, got %d", ht.Size())
	}
	if ht.Capacity() != 16 {
		t.Errorf("expected capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableOAInsert(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)
	ht.Insert(3, 300)

	if ht.Size() != 3 {
		t.Errorf("expected size 3, got %d", ht.Size())
	}
}

func TestHashTableOAGet(t *testing.T) {
	ht := NewHashTableOA(16)
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

func TestHashTableOAUpdate(t *testing.T) {
	ht := NewHashTableOA(16)
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

func TestHashTableOARemove(t *testing.T) {
	ht := NewHashTableOA(16)
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

func TestHashTableOAResize(t *testing.T) {
	ht := NewHashTableOA(4)
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

func TestHashTableOASaveLoadText(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(1, 100)
	ht.Insert(2, 200)
	ht.Insert(3, 300)

	filename := "test_ht_oa.txt"
	defer os.Remove(filename)

	if err := ht.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	ht2 := NewHashTableOA(16)
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

func TestHashTableOASaveLoadBinary(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 1000)
	ht.Insert(20, 2000)
	ht.Insert(30, 3000)

	filename := "test_ht_oa.bin"
	defer os.Remove(filename)

	if err := ht.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	ht2 := NewHashTableOA(16)
	if err := ht2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if ht2.Size() != ht.Size() {
		t.Errorf("size mismatch: expected %d, got %d", ht.Size(), ht2.Size())
	}
}

func TestHashTableOANewZeroCapacity(t *testing.T) {
	ht := NewHashTableOA(0)
	if ht.Capacity() != 16 {
		t.Errorf("Expected default capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableOANewNegativeCapacity(t *testing.T) {
	ht := NewHashTableOA(-5)
	if ht.Capacity() != 16 {
		t.Errorf("Expected default capacity 16, got %d", ht.Capacity())
	}
}

func TestHashTableOAInsertUpdate(t *testing.T) {
	ht := NewHashTableOA(16)
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

func TestHashTableOAGetNotFound(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	
	_, err := ht.Get(999)
	if err == nil {
		t.Error("Expected error for non-existent key")
	}
}

func TestHashTableOARemoveNotFound(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	
	err := ht.Remove(999)
	if err == nil {
		t.Error("Expected error when removing non-existent key")
	}
}

func TestHashTableOAContainsTrue(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	
	if !ht.Contains(10) {
		t.Error("Expected Contains(10) to be true")
	}
}

func TestHashTableOAContainsFalse(t *testing.T) {
	ht := NewHashTableOA(16)
	
	if ht.Contains(10) {
		t.Error("Expected Contains(10) to be false")
	}
}

func TestHashTableOAClear(t *testing.T) {
	ht := NewHashTableOA(16)
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

func TestHashTableOAPrint(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	ht.Insert(20, 200)
	ht.Print()
}

func TestHashTableOASaveTextError(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	
	err := ht.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestHashTableOALoadTextError(t *testing.T) {
	ht := NewHashTableOA(16)
	err := ht.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestHashTableOASaveBinaryError(t *testing.T) {
	ht := NewHashTableOA(16)
	ht.Insert(10, 100)
	
	err := ht.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestHashTableOALoadBinaryError(t *testing.T) {
	ht := NewHashTableOA(16)
	err := ht.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestHashTableOACollisionHandling(t *testing.T) {
	ht := NewHashTableOA(16)
	
	// Insert multiple elements that might collide
	for i := 0; i < 20; i++ {
		ht.Insert(i, i*10)
	}
	
	// Verify all can be retrieved
	for i := 0; i < 20; i++ {
		val, err := ht.Get(i)
		if err != nil {
			t.Errorf("Get(%d) failed: %v", i, err)
		}
		if val != i*10 {
			t.Errorf("Expected %d, got %d", i*10, val)
		}
	}
}
