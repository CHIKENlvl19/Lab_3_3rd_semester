package datastructures

import (
	"os"
	"testing"
)

func TestSListNew(t *testing.T) {
	sl := NewSinglyLinkedList()
	if sl.Size() != 0 {
		t.Errorf("Expected size 0, got %d", sl.Size())
	}
	if !sl.IsEmpty() {
		t.Error("New list should be empty")
	}
}

func TestSListAddHead(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddHead(10)
	sl.AddHead(20)
	
	if sl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", sl.Size())
	}
	if sl.IsEmpty() {
		t.Error("List should not be empty")
	}
}

func TestSListAddHeadSingle(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddHead(10)
	
	if sl.head != sl.tail {
		t.Error("For single element, head should equal tail")
	}
}

func TestSListAddTail(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	
	if sl.Size() != 3 {
		t.Errorf("Expected size 3, got %d", sl.Size())
	}
}

func TestSListAddTailEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	
	if sl.head != sl.tail {
		t.Error("For single element, head should equal tail")
	}
}

func TestSListRemoveHead(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	
	err := sl.RemoveHead()
	if err != nil {
		t.Errorf("RemoveHead failed: %v", err)
	}
	
	if sl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", sl.Size())
	}
}

func TestSListRemoveHeadEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	err := sl.RemoveHead()
	if err == nil {
		t.Error("Expected error when removing from empty list")
	}
}

func TestSListRemoveHeadSingle(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	
	sl.RemoveHead()
	
	if sl.Size() != 0 {
		t.Errorf("Expected size 0, got %d", sl.Size())
	}
	if sl.head != nil || sl.tail != nil {
		t.Error("Head and tail should be nil after removing last element")
	}
}

func TestSListRemoveTail(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	
	err := sl.RemoveTail()
	if err != nil {
		t.Errorf("RemoveTail failed: %v", err)
	}
	
	if sl.Size() != 2 {
		t.Errorf("Expected size 2, got %d", sl.Size())
	}
}

func TestSListRemoveTailEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	err := sl.RemoveTail()
	if err == nil {
		t.Error("Expected error when removing from empty list")
	}
}

func TestSListRemoveTailSingle(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	
	err := sl.RemoveTail()
	if err != nil {
		t.Errorf("RemoveTail failed: %v", err)
	}
	
	if sl.Size() != 0 {
		t.Errorf("Expected size 0, got %d", sl.Size())
	}
	if sl.head != nil || sl.tail != nil {
		t.Error("Head and tail should be nil")
	}
}

func TestSListSearch(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	
	index := sl.Search(20)
	if index != 1 {
		t.Errorf("Expected index 1, got %d", index)
	}
	
	index = sl.Search(10)
	if index != 0 {
		t.Errorf("Expected index 0, got %d", index)
	}
	
	index = sl.Search(30)
	if index != 2 {
		t.Errorf("Expected index 2, got %d", index)
	}
}

func TestSListSearchNotFound(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	
	index := sl.Search(999)
	if index != -1 {
		t.Errorf("Expected -1 for not found, got %d", index)
	}
}

func TestSListClear(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	
	sl.Clear()
	
	if sl.Size() != 0 {
		t.Errorf("Expected size 0 after clear, got %d", sl.Size())
	}
	if !sl.IsEmpty() {
		t.Error("List should be empty after clear")
	}
	if sl.head != nil || sl.tail != nil {
		t.Error("Head and tail should be nil after clear")
	}
}

func TestSListPrintEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.Print() // Should print []
}

func TestSListPrint(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	sl.AddTail(20)
	sl.AddTail(30)
	sl.Print()
}

func TestSListSaveLoadText(t *testing.T) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 10; i++ {
		sl.AddTail(i * 10)
	}
	
	filename := "test_slist.txt"
	defer os.Remove(filename)
	
	err := sl.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}
	
	sl2 := NewSinglyLinkedList()
	err = sl2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}
	
	if sl2.Size() != sl.Size() {
		t.Errorf("Size mismatch: expected %d, got %d", sl.Size(), sl2.Size())
	}
	
	// Проверяем значения
	curr1 := sl.head
	curr2 := sl2.head
	for curr1 != nil && curr2 != nil {
		if curr1.value != curr2.value {
			t.Errorf("Value mismatch: expected %d, got %d", curr1.value, curr2.value)
		}
		curr1 = curr1.next
		curr2 = curr2.next
	}
}

func TestSListSaveTextEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	filename := "test_slist_empty.txt"
	defer os.Remove(filename)
	
	err := sl.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}
	
	sl2 := NewSinglyLinkedList()
	err = sl2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}
	
	if sl2.Size() != 0 {
		t.Errorf("Expected empty list, got size %d", sl2.Size())
	}
}

func TestSListSaveTextError(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	
	err := sl.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestSListLoadTextError(t *testing.T) {
	sl := NewSinglyLinkedList()
	err := sl.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestSListSaveLoadBinary(t *testing.T) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 10; i++ {
		sl.AddTail(i * 10)
	}
	
	filename := "test_slist.bin"
	defer os.Remove(filename)
	
	err := sl.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}
	
	sl2 := NewSinglyLinkedList()
	err = sl2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}
	
	if sl2.Size() != sl.Size() {
		t.Errorf("Size mismatch: expected %d, got %d", sl.Size(), sl2.Size())
	}
	
	// Проверяем значения
	curr1 := sl.head
	curr2 := sl2.head
	for curr1 != nil && curr2 != nil {
		if curr1.value != curr2.value {
			t.Errorf("Value mismatch: expected %d, got %d", curr1.value, curr2.value)
		}
		curr1 = curr1.next
		curr2 = curr2.next
	}
}

func TestSListSaveBinaryEmpty(t *testing.T) {
	sl := NewSinglyLinkedList()
	filename := "test_slist_empty.bin"
	defer os.Remove(filename)
	
	err := sl.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}
	
	sl2 := NewSinglyLinkedList()
	err = sl2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}
	
	if sl2.Size() != 0 {
		t.Errorf("Expected empty list, got size %d", sl2.Size())
	}
}

func TestSListSaveBinaryError(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.AddTail(10)
	
	err := sl.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestSListLoadBinaryError(t *testing.T) {
	sl := NewSinglyLinkedList()
	err := sl.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestSListMixed(t *testing.T) {
	sl := NewSinglyLinkedList()
	
	sl.AddHead(20)
	sl.AddTail(30)
	sl.AddHead(10)
	
	// Должно быть: 10 -> 20 -> 30
	if sl.Size() != 3 {
		t.Errorf("Expected size 3, got %d", sl.Size())
	}
	
	if sl.Search(10) != 0 {
		t.Error("10 should be at index 0")
	}
	if sl.Search(20) != 1 {
		t.Error("20 should be at index 1")
	}
	if sl.Search(30) != 2 {
		t.Error("30 should be at index 2")
	}
}
