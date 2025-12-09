package datastructures

import (
	"os"
	"testing"
)

func TestQueueNew(t *testing.T) {
	q := NewQueue(10)
	if q.Size() != 0 {
		t.Errorf("expected size 0, got %d", q.Size())
	}
	if q.Capacity() != 10 {
		t.Errorf("expected capacity 10, got %d", q.Capacity())
	}
}

func TestQueuePush(t *testing.T) {
	q := NewQueue(4)
	q.Push(1)
	q.Push(2)
	q.Push(3)

	if q.Size() != 3 {
		t.Errorf("expected size 3, got %d", q.Size())
	}

	val, _ := q.Front()
	if val != 1 {
		t.Errorf("expected front value 1, got %d", val)
	}
}

func TestQueuePop(t *testing.T) {
	q := NewQueue(4)
	q.Push(1)
	q.Push(2)
	q.Push(3)

	q.Pop()
	if q.Size() != 2 {
		t.Errorf("expected size 2, got %d", q.Size())
	}

	val, _ := q.Front()
	if val != 2 {
		t.Errorf("expected front value 2, got %d", val)
	}
}

func TestQueueResize(t *testing.T) {
	q := NewQueue(2)
	q.Push(1)
	q.Push(2)
	q.Push(3) // Should trigger resize

	if q.Size() != 3 {
		t.Errorf("expected size 3, got %d", q.Size())
	}
	if q.Capacity() < 3 {
		t.Errorf("expected capacity >= 3, got %d", q.Capacity())
	}
}

func TestQueueSaveLoadText(t *testing.T) {
	q := NewQueue(4)
	q.Push(10)
	q.Push(20)
	q.Push(30)

	filename := "test_queue.txt"
	defer os.Remove(filename)

	if err := q.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	q2 := NewQueue(4)
	if err := q2.LoadText(filename); err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if q2.Size() != q.Size() {
		t.Errorf("size mismatch: expected %d, got %d", q.Size(), q2.Size())
	}
}

func TestQueueSaveLoadBinary(t *testing.T) {
	q := NewQueue(4)
	q.Push(100)
	q.Push(200)
	q.Push(300)

	filename := "test_queue.bin"
	defer os.Remove(filename)

	if err := q.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	q2 := NewQueue(4)
	if err := q2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if q2.Size() != q.Size() {
		t.Errorf("size mismatch: expected %d, got %d", q.Size(), q2.Size())
	}
}

func TestQueueNewZeroCapacity(t *testing.T) {
	q := NewQueue(0)
	if q.Capacity() < 4 {
		t.Errorf("Expected minimum capacity 4, got %d", q.Capacity())
	}
}

func TestQueueNewNegativeCapacity(t *testing.T) {
	q := NewQueue(-5)
	if q.Capacity() < 4 {
		t.Errorf("Expected minimum capacity 4, got %d", q.Capacity())
	}
}

func TestQueuePopEmpty(t *testing.T) {
	q := NewQueue(4)
	err := q.Pop()
	if err == nil {
		t.Error("Expected error when popping empty queue")
	}
}

func TestQueueFrontEmpty(t *testing.T) {
	q := NewQueue(4)
	_, err := q.Front()
	if err == nil {
		t.Error("Expected error when accessing front of empty queue")
	}
}

func TestQueueCompact(t *testing.T) {
	q := NewQueue(4)
	// Добавляем много элементов
	for i := 0; i < 100; i++ {
		q.Push(i)
	}
	
	// Удаляем много элементов чтобы вызвать compact
	for i := 0; i < 60; i++ {
		q.Pop()
	}
	
	// Проверяем что очередь все еще работает
	val, err := q.Front()
	if err != nil {
		t.Errorf("Front failed after compact: %v", err)
	}
	if val != 60 {
		t.Errorf("Expected 60, got %d", val)
	}
}

func TestQueuePrint(t *testing.T) {
	q := NewQueue(4)
	q.Print() // Empty queue
	
	q.Push(10)
	q.Push(20)
	q.Print() // Non-empty queue
}

func TestQueueSaveTextError(t *testing.T) {
	q := NewQueue(4)
	q.Push(10)
	
	err := q.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestQueueLoadTextError(t *testing.T) {
	q := NewQueue(4)
	err := q.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestQueueSaveBinaryError(t *testing.T) {
	q := NewQueue(4)
	q.Push(10)
	
	err := q.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestQueueLoadBinaryError(t *testing.T) {
	q := NewQueue(4)
	err := q.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}
