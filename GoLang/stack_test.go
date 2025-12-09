package datastructures

import (
	"os"
	"testing"
)

func TestStackNew(t *testing.T) {
	s := NewStack()
	if s.Size() != 0 {
		t.Errorf("Expected size 0, got %d", s.Size())
	}
	if !s.IsEmpty() {
		t.Error("New stack should be empty")
	}
}

func TestStackPush(t *testing.T) {
	s := NewStack()
	err := s.Push(10)
	if err != nil {
		t.Errorf("Push failed: %v", err)
	}
	if s.Size() != 1 {
		t.Errorf("Expected size 1, got %d", s.Size())
	}
	if s.IsEmpty() {
		t.Error("Stack should not be empty after push")
	}
}

func TestStackPushMultiple(t *testing.T) {
	s := NewStack()
	for i := 0; i < 100; i++ {
		s.Push(i)
	}
	if s.Size() != 100 {
		t.Errorf("Expected size 100, got %d", s.Size())
	}
}

func TestStackPushOverflow(t *testing.T) {
	s := NewStack()
	// Пытаемся переполнить стек
	for i := 0; i < MaxStackSize; i++ {
		s.Push(i)
	}
	err := s.Push(999)
	if err == nil {
		t.Error("Expected overflow error, got nil")
	}
}

func TestStackPop(t *testing.T) {
	s := NewStack()
	s.Push(10)
	s.Push(20)
	err := s.Pop()
	if err != nil {
		t.Errorf("Pop failed: %v", err)
	}
	if s.Size() != 1 {
		t.Errorf("Expected size 1, got %d", s.Size())
	}
}

func TestStackPopEmpty(t *testing.T) {
	s := NewStack()
	err := s.Pop()
	if err == nil {
		t.Error("Expected error when popping empty stack")
	}
}

func TestStackPeek(t *testing.T) {
	s := NewStack()
	s.Push(10)
	s.Push(20)
	val, err := s.Peek()
	if err != nil {
		t.Errorf("Peek failed: %v", err)
	}
	if val != 20 {
		t.Errorf("Expected 20, got %d", val)
	}
	if s.Size() != 2 {
		t.Error("Peek should not change size")
	}
}

func TestStackPeekEmpty(t *testing.T) {
	s := NewStack()
	_, err := s.Peek()
	if err == nil {
		t.Error("Expected error when peeking empty stack")
	}
}

func TestStackClear(t *testing.T) {
	s := NewStack()
	s.Push(10)
	s.Push(20)
	s.Push(30)
	s.Clear()
	if s.Size() != 0 {
		t.Errorf("Expected size 0 after clear, got %d", s.Size())
	}
	if !s.IsEmpty() {
		t.Error("Stack should be empty after clear")
	}
}

func TestStackPrint(t *testing.T) {
	s := NewStack()
	s.Print() // Empty stack
	s.Push(10)
	s.Push(20)
	s.Print() // Non-empty stack
}

func TestStackSaveLoadText(t *testing.T) {
	s := NewStack()
	for i := 0; i < 10; i++ {
		s.Push(i * 10)
	}

	filename := "test_stack.txt"
	defer os.Remove(filename)

	err := s.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	s2 := NewStack()
	err = s2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if s2.Size() != s.Size() {
		t.Errorf("Size mismatch: expected %d, got %d", s.Size(), s2.Size())
	}

	for i := 0; i < s.Size(); i++ {
		if s.data[i] != s2.data[i] {
			t.Errorf("Data mismatch at index %d", i)
		}
	}
}

func TestStackSaveTextEmpty(t *testing.T) {
	s := NewStack()
	filename := "test_stack_empty.txt"
	defer os.Remove(filename)

	err := s.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	s2 := NewStack()
	err = s2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if s2.Size() != 0 {
		t.Errorf("Expected empty stack, got size %d", s2.Size())
	}
}

func TestStackLoadTextError(t *testing.T) {
	s := NewStack()
	err := s.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestStackSaveTextError(t *testing.T) {
	s := NewStack()
	s.Push(10)
	err := s.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestStackSaveLoadBinary(t *testing.T) {
	s := NewStack()
	for i := 0; i < 10; i++ {
		s.Push(i * 10)
	}

	filename := "test_stack.bin"
	defer os.Remove(filename)

	err := s.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	s2 := NewStack()
	err = s2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if s2.Size() != s.Size() {
		t.Errorf("Size mismatch: expected %d, got %d", s.Size(), s2.Size())
	}

	for i := 0; i < s.Size(); i++ {
		if s.data[i] != s2.data[i] {
			t.Errorf("Data mismatch at index %d", i)
		}
	}
}

func TestStackSaveBinaryEmpty(t *testing.T) {
	s := NewStack()
	filename := "test_stack_empty.bin"
	defer os.Remove(filename)

	err := s.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	s2 := NewStack()
	err = s2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if s2.Size() != 0 {
		t.Errorf("Expected empty stack, got size %d", s2.Size())
	}
}

func TestStackLoadBinaryError(t *testing.T) {
	s := NewStack()
	err := s.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestStackSaveBinaryError(t *testing.T) {
	s := NewStack()
	s.Push(10)
	err := s.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestStackLIFO(t *testing.T) {
	s := NewStack()
	s.Push(1)
	s.Push(2)
	s.Push(3)

	val, _ := s.Peek()
	if val != 3 {
		t.Errorf("Expected 3, got %d", val)
	}

	s.Pop()
	val, _ = s.Peek()
	if val != 2 {
		t.Errorf("Expected 2, got %d", val)
	}

	s.Pop()
	val, _ = s.Peek()
	if val != 1 {
		t.Errorf("Expected 1, got %d", val)
	}
}
