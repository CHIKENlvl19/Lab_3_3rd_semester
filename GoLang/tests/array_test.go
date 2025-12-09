package tests

import (
	"os"
	"testing"
	"github.com/CHIKENlvl19/Lab_3_3rd_semester/structures"
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
