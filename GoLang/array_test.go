package datastructures

import (
	"os"
	"testing"
)

func TestNew(t *testing.T) {
	arr := New(10)
	if arr.Size() != 0 {
		t.Errorf("expected size 0, got %d", arr.Size())
	}
	if arr.Capacity() != 10 {
		t.Errorf("expected capacity 10, got %d", arr.Capacity())
	}
}

func TestPushBack(t *testing.T) {
	arr := New(4)
	arr.PushBack(1)
	arr.PushBack(2)
	arr.PushBack(3)

	if arr.Size() != 3 {
		t.Errorf("expected size 3, got %d", arr.Size())
	}

	val, _ := arr.Get(0)
	if val != 1 {
		t.Errorf("expected value 1, got %d", val)
	}
}

func TestInsert(t *testing.T) {
	arr := New(4)
	arr.PushBack(1)
	arr.PushBack(3)
	arr.Insert(1, 2)

	if arr.Size() != 3 {
		t.Errorf("expected size 3, got %d", arr.Size())
	}

	val, _ := arr.Get(1)
	if val != 2 {
		t.Errorf("expected value 2 at index 1, got %d", val)
	}
}

func TestRemove(t *testing.T) {
	arr := New(4)
	arr.PushBack(1)
	arr.PushBack(2)
	arr.PushBack(3)
	arr.Remove(1)

	if arr.Size() != 2 {
		t.Errorf("expected size 2, got %d", arr.Size())
	}

	val, _ := arr.Get(1)
	if val != 3 {
		t.Errorf("expected value 3 at index 1, got %d", val)
	}
}

func TestSaveLoadText(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	arr.PushBack(20)
	arr.PushBack(30)

	filename := "test_array.txt"
	defer os.Remove(filename)

	if err := arr.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	arr2 := New(4)
	if err := arr2.LoadText(filename); err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if arr2.Size() != arr.Size() {
		t.Errorf("size mismatch: expected %d, got %d", arr.Size(), arr2.Size())
	}

	for i := 0; i < arr.Size(); i++ {
		v1, _ := arr.Get(i)
		v2, _ := arr2.Get(i)
		if v1 != v2 {
			t.Errorf("value mismatch at index %d: expected %d, got %d", i, v1, v2)
		}
	}
}

func TestSaveLoadBinary(t *testing.T) {
	arr := New(4)
	arr.PushBack(100)
	arr.PushBack(200)
	arr.PushBack(300)

	filename := "test_array.bin"
	defer os.Remove(filename)

	if err := arr.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	arr2 := New(4)
	if err := arr2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if arr2.Size() != arr.Size() {
		t.Errorf("size mismatch: expected %d, got %d", arr.Size(), arr2.Size())
	}

	for i := 0; i < arr.Size(); i++ {
		v1, _ := arr.Get(i)
		v2, _ := arr2.Get(i)
		if v1 != v2 {
			t.Errorf("value mismatch at index %d: expected %d, got %d", i, v1, v2)
		}
	}
}

func TestArrayNewZeroCapacity(t *testing.T) {
	arr := New(0)
	if arr.Capacity() != 4 {
		t.Errorf("Expected default capacity 4, got %d", arr.Capacity())
	}
}

func TestArrayNewNegativeCapacity(t *testing.T) {
	arr := New(-5)
	if arr.Capacity() != 4 {
		t.Errorf("Expected default capacity 4, got %d", arr.Capacity())
	}
}

func TestArrayGetOutOfRange(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	_, err := arr.Get(-1)
	if err == nil {
		t.Error("Expected error for negative index")
	}
	
	_, err = arr.Get(10)
	if err == nil {
		t.Error("Expected error for index >= size")
	}
}

func TestArraySetOutOfRange(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	err := arr.Set(-1, 100)
	if err == nil {
		t.Error("Expected error for negative index")
	}
	
	err = arr.Set(10, 100)
	if err == nil {
		t.Error("Expected error for index >= size")
	}
}

func TestArraySet(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	arr.PushBack(20)
	
	err := arr.Set(0, 100)
	if err != nil {
		t.Errorf("Set failed: %v", err)
	}
	
	val, _ := arr.Get(0)
	if val != 100 {
		t.Errorf("Expected 100, got %d", val)
	}
}

func TestArrayPushBackResize(t *testing.T) {
	arr := New(2)
	arr.PushBack(1)
	arr.PushBack(2)
	arr.PushBack(3) // Should trigger resize
	
	if arr.Capacity() != 4 {
		t.Errorf("Expected capacity 4 after resize, got %d", arr.Capacity())
	}
}

func TestArrayInsertOutOfRange(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	err := arr.Insert(-1, 100)
	if err == nil {
		t.Error("Expected error for negative index")
	}
	
	err = arr.Insert(10, 100)
	if err == nil {
		t.Error("Expected error for index > size")
	}
}

func TestArrayInsertAtEnd(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	arr.PushBack(20)
	
	err := arr.Insert(2, 30)
	if err != nil {
		t.Errorf("Insert at end failed: %v", err)
	}
	
	val, _ := arr.Get(2)
	if val != 30 {
		t.Errorf("Expected 30, got %d", val)
	}
}

func TestArrayInsertResize(t *testing.T) {
	arr := New(2)
	arr.PushBack(1)
	arr.PushBack(2)
	
	err := arr.Insert(1, 99) // Should trigger resize
	if err != nil {
		t.Errorf("Insert with resize failed: %v", err)
	}
	
	if arr.Capacity() != 4 {
		t.Errorf("Expected capacity 4 after resize, got %d", arr.Capacity())
	}
}

func TestArrayRemoveOutOfRange(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	err := arr.Remove(-1)
	if err == nil {
		t.Error("Expected error for negative index")
	}
	
	err = arr.Remove(10)
	if err == nil {
		t.Error("Expected error for index >= size")
	}
}

func TestArrayPrint(t *testing.T) {
	arr := New(4)
	arr.Print() // Empty array
	
	arr.PushBack(10)
	arr.PushBack(20)
	arr.Print() // Non-empty array
}

func TestArraySaveTextError(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	err := arr.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestArrayLoadTextError(t *testing.T) {
	arr := New(4)
	err := arr.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestArraySaveBinaryError(t *testing.T) {
	arr := New(4)
	arr.PushBack(10)
	
	err := arr.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestArrayLoadBinaryError(t *testing.T) {
	arr := New(4)
	err := arr.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}
