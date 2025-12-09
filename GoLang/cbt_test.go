package datastructures

import (
	"os"
	"testing"
)

func TestCBTNew(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	if cbt.Size() != 0 {
		t.Errorf("expected size 0, got %d", cbt.Size())
	}
	if !cbt.IsEmpty() {
		t.Error("expected tree to be empty")
	}
}

func TestCBTInsert(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(1)
	cbt.Insert(2)
	cbt.Insert(3)

	if cbt.Size() != 3 {
		t.Errorf("expected size 3, got %d", cbt.Size())
	}

	root, err := cbt.GetRoot()
	if err != nil {
		t.Fatalf("GetRoot failed: %v", err)
	}
	if root != 1 {
		t.Errorf("expected root value 1, got %d", root)
	}
}

func TestCBTRemoveLast(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(1)
	cbt.Insert(2)
	cbt.Insert(3)

	if err := cbt.RemoveLast(); err != nil {
		t.Fatalf("RemoveLast failed: %v", err)
	}

	if cbt.Size() != 2 {
		t.Errorf("expected size 2, got %d", cbt.Size())
	}
}

func TestCBTRemoveLastEmpty(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	err := cbt.RemoveLast()
	if err == nil {
		t.Error("expected error when removing from empty tree")
	}
}

func TestCBTInsertMany(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	for i := 1; i <= 7; i++ {
		cbt.Insert(i * 10)
	}

	if cbt.Size() != 7 {
		t.Errorf("expected size 7, got %d", cbt.Size())
	}
}

func TestCBTSaveLoadText(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(10)
	cbt.Insert(20)
	cbt.Insert(30)
	cbt.Insert(40)

	filename := "test_cbt.txt"
	defer os.Remove(filename)

	if err := cbt.SaveText(filename); err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}

	cbt2 := NewCompleteBinaryTree()
	if err := cbt2.LoadText(filename); err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}

	if cbt2.Size() != cbt.Size() {
		t.Errorf("size mismatch: expected %d, got %d", cbt.Size(), cbt2.Size())
	}

	root1, _ := cbt.GetRoot()
	root2, _ := cbt2.GetRoot()
	if root1 != root2 {
		t.Errorf("root mismatch: expected %d, got %d", root1, root2)
	}
}

func TestCBTSaveLoadBinary(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(100)
	cbt.Insert(200)
	cbt.Insert(300)
	cbt.Insert(400)

	filename := "test_cbt.bin"
	defer os.Remove(filename)

	if err := cbt.SaveBinary(filename); err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}

	cbt2 := NewCompleteBinaryTree()
	if err := cbt2.LoadBinary(filename); err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}

	if cbt2.Size() != cbt.Size() {
		t.Errorf("size mismatch: expected %d, got %d", cbt.Size(), cbt2.Size())
	}
}

func TestCBTGetRootEmpty(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	_, err := cbt.GetRoot()
	if err == nil {
		t.Error("Expected error when getting root of empty tree")
	}
}

func TestCBTGetRoot(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(10)
	cbt.Insert(20)
	
	val, err := cbt.GetRoot()
	if err != nil {
		t.Errorf("GetRoot failed: %v", err)
	}
	if val != 10 {
		t.Errorf("Expected root 10, got %d", val)
	}
}

func TestCBTRemoveLastSingleElement(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(10)
	
	err := cbt.RemoveLast()
	if err != nil {
		t.Errorf("RemoveLast failed: %v", err)
	}
	
	if cbt.Size() != 0 {
		t.Errorf("Expected size 0, got %d", cbt.Size())
	}
	
	if !cbt.IsEmpty() {
		t.Error("Tree should be empty after removing last element")
	}
}

func TestCBTRemoveLastMultiple(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 10; i++ {
		cbt.Insert(i)
	}
	
	cbt.RemoveLast()
	if cbt.Size() != 9 {
		t.Errorf("Expected size 9, got %d", cbt.Size())
	}
	
	cbt.RemoveLast()
	if cbt.Size() != 8 {
		t.Errorf("Expected size 8, got %d", cbt.Size())
	}
}

func TestCBTPrintLevelOrderEmpty(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.PrintLevelOrder() // Should print []
}

func TestCBTPrintLevelOrder(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(1)
	cbt.Insert(2)
	cbt.Insert(3)
	cbt.PrintLevelOrder()
}

func TestCBTClear(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 10; i++ {
		cbt.Insert(i)
	}
	
	cbt.Clear()
	if cbt.Size() != 0 {
		t.Errorf("Expected size 0 after clear, got %d", cbt.Size())
	}
	if !cbt.IsEmpty() {
		t.Error("Tree should be empty after clear")
	}
}

func TestCBTSaveTextEmpty(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	filename := "test_cbt_empty.txt"
	defer os.Remove(filename)
	
	err := cbt.SaveText(filename)
	if err != nil {
		t.Fatalf("SaveText failed: %v", err)
	}
	
	cbt2 := NewCompleteBinaryTree()
	err = cbt2.LoadText(filename)
	if err != nil {
		t.Fatalf("LoadText failed: %v", err)
	}
	
	if cbt2.Size() != 0 {
		t.Errorf("Expected empty tree, got size %d", cbt2.Size())
	}
}

func TestCBTSaveTextError(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(10)
	
	err := cbt.SaveText("/invalid/path/file.txt")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestCBTLoadTextError(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	err := cbt.LoadText("nonexistent_file.txt")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestCBTSaveBinaryEmpty(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	filename := "test_cbt_empty.bin"
	defer os.Remove(filename)
	
	err := cbt.SaveBinary(filename)
	if err != nil {
		t.Fatalf("SaveBinary failed: %v", err)
	}
	
	cbt2 := NewCompleteBinaryTree()
	err = cbt2.LoadBinary(filename)
	if err != nil {
		t.Fatalf("LoadBinary failed: %v", err)
	}
	
	if cbt2.Size() != 0 {
		t.Errorf("Expected empty tree, got size %d", cbt2.Size())
	}
}

func TestCBTSaveBinaryError(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	cbt.Insert(10)
	
	err := cbt.SaveBinary("/invalid/path/file.bin")
	if err == nil {
		t.Error("Expected error for invalid path")
	}
}

func TestCBTLoadBinaryError(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	err := cbt.LoadBinary("nonexistent_file.bin")
	if err == nil {
		t.Error("Expected error for nonexistent file")
	}
}

func TestCBTLargeTree(t *testing.T) {
	cbt := NewCompleteBinaryTree()
	n := 1000
	
	for i := 0; i < n; i++ {
		cbt.Insert(i)
	}
	
	if cbt.Size() != n {
		t.Errorf("Expected size %d, got %d", n, cbt.Size())
	}
	
	for i := 0; i < n; i++ {
		cbt.RemoveLast()
	}
	
	if cbt.Size() != 0 {
		t.Errorf("Expected size 0, got %d", cbt.Size())
	}
}
