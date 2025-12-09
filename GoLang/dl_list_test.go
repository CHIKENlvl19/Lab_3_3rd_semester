package datastructures

import (
	"os"
	"testing"
)

func TestDListNew(t *testing.T) {
	dl := NewDoublyLinkedList()
	if dl.Size() != 0 {
		t.Errorf("expected size 0, got %d", dl.Size())
	}
	if !dl.IsEmpty() {
		t.Error("expected list to be empty")
	}
}

func TestDListAddHead(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddHead(1)
	dl.AddHead(2)
	dl.AddHead(3)

	if dl.Size() != 3 {
		t.Errorf("expected size 3, got %d", dl.Size())
	}

	if dl.head.value != 3 {
		t.Errorf("expected head value 3, got %d", dl.head.value)
	}

	if dl.tail.value != 1 {
		t.Errorf("expected tail value 1, got %d", dl.tail.value)
	}
}

func TestDListAddTail(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(1)
	dl.AddTail(2)
	dl.AddTail(3)

	if dl.Size() != 3 {
		t.Errorf("expected size 3, got %d", dl.Size())
	}

	if dl.head.value != 1 {
		t.Errorf("expected head value 1, got %d", dl.head.value)
	}

	if dl.tail.value != 3 {
		t.Errorf("expected tail value 3, got %d", dl.tail.value)
	}
}

func TestDListRemoveByValue(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(1)
	dl.AddTail(2)
	dl.AddTail(3)

	if !dl.RemoveByValue(2) {
		t.Error("failed to remove value 2")
	}

	if dl.Size() != 2 {
		t.Errorf("expected size 2, got %d", dl.Size())
	}

	if dl.Search(2) != -1 {
		t.Error("value 2 should not exist after removal")
	}
}

func TestDListSearch(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)

	index := dl.Search(20)
	if index != 1 {
		t.Errorf("expected index 1, got %d", index)
	}

	index = dl.Search(99)
	if index != -1 {
		t.Errorf("expected index -1, got %d", index)
	}
}

func TestDListSaveLoadText(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)

	filename := "test_dlist.txt"
	defer os.Remove(filename)

	if err := dl.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	dl2 := NewDoublyLinkedList()
	if err := dl2.LoadText(filename); err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if dl2.Size() != dl.Size() {
		t.Errorf("size mismatch: expected %d, got %d", dl.Size(), dl2.Size())
	}
}

func TestDListSaveLoadBinary(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(100)
	dl.AddTail(200)
	dl.AddTail(300)

	filename := "test_dlist.bin"
	defer os.Remove(filename)

	if err := dl.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	dl2 := NewDoublyLinkedList()
	if err := dl2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if dl2.Size() != dl.Size() {
		t.Errorf("size mismatch: expected %d, got %d", dl.Size(), dl2.Size())
	}
}

func TestDListAddHeadEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddHead(10)
	
	if dl.Size() != 1 {
		t.Errorf("Expected size 1, got %d", dl.Size())
	}
	
	// Проверяем что head и tail указывают на один узел
	if dl.head != dl.tail {
		t.Error("For single element, head should equal tail")
	}
}

func TestDListAddTailEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	
	if dl.Size() != 1 {
		t.Errorf("Expected size 1, got %d", dl.Size())
	}
	
	if dl.head != dl.tail {
		t.Error("For single element, head should equal tail")
	}
}

func TestDListAddAfterOutOfRange(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	
	err := dl.AddAfter(-1, 100)
	if err == nil {
		t.Error("Expected error for negative index")
	}
	
	err = dl.AddAfter(10, 100)
	if err == nil {
		t.Error("Expected error for index > size")
	}
}

func TestDListAddAfterAtEnd(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	
	err := dl.AddAfter(2, 30)
	if err != nil {
		t.Errorf("AddAfter at end failed: %v", err)
	}
	
	if dl.Size() != 3 {
		t.Errorf("Expected size 3, got %d", dl.Size())
	}
}

func TestDListAddAfterMiddle(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)
	
	err := dl.AddAfter(1, 99)
	if err != nil {
		t.Errorf("AddAfter failed: %v", err)
	}
	
	if dl.Size() != 4 {
		t.Errorf("Expected size 4, got %d", dl.Size())
	}
}

func TestDListRemoveByValueHead(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)
	
	found := dl.RemoveByValue(10)
	if !found {
		t.Error("Expected to find and remove value 10")
	}
	
	if dl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", dl.Size())
	}
}

func TestDListRemoveByValueTail(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)
	
	found := dl.RemoveByValue(30)
	if !found {
		t.Error("Expected to find and remove value 30")
	}
	
	if dl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", dl.Size())
	}
}

func TestDListRemoveByValueMiddle(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)
	
	found := dl.RemoveByValue(20)
	if !found {
		t.Error("Expected to find and remove value 20")
	}
	
	if dl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", dl.Size())
	}
}

func TestDListRemoveByValueNotFound(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	
	found := dl.RemoveByValue(999)
	if found {
		t.Error("Expected not to find value 999")
	}
	
	if dl.Size() != 2 {
		t.Errorf("Size should not change, got %d", dl.Size())
	}
}

func TestDListSearchNotFound(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	
	index := dl.Search(999)
	if index != -1 {
		t.Errorf("Expected -1 for not found, got %d", index)
	}
}

func TestDListPrintEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.Print() // Should print []
}

func TestDListPrint(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.Print()
}

func TestDListPrintBackwardsEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.PrintBackwards() // Should print []
}

func TestDListPrintBackwards(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	dl.AddTail(20)
	dl.AddTail(30)
	dl.PrintBackwards()
}

func TestDListSaveTextEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	filename := "test_dlist_empty.txt"
	defer os.Remove(filename)
	
	err := dl.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}
	
	dl2 := NewDoublyLinkedList()
	err = dl2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}
	
	if dl2.Size() != 0 {
		t.Errorf("Expected empty list, got size %d", dl2.Size())
	}
}

func TestDListSaveTextError(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	
	err := dl.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestDListLoadTextError(t *testing.T) {
	dl := NewDoublyLinkedList()
	err := dl.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestDListSaveBinaryEmpty(t *testing.T) {
	dl := NewDoublyLinkedList()
	filename := "test_dlist_empty.bin"
	defer os.Remove(filename)
	
	err := dl.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}
	
	dl2 := NewDoublyLinkedList()
	err = dl2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}
	
	if dl2.Size() != 0 {
		t.Errorf("Expected empty list, got size %d", dl2.Size())
	}
}

func TestDListSaveBinaryError(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.AddTail(10)
	
	err := dl.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestDListLoadBinaryError(t *testing.T) {
	dl := NewDoublyLinkedList()
	err := dl.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}
