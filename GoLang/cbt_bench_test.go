package datastructures

import (
	"os"
	"testing"
)

func BenchmarkCBTInsert(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt.Insert(i)
	}
}

func BenchmarkCBTRemoveLast(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < b.N; i++ {
		cbt.Insert(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt.RemoveLast()
	}
}

func BenchmarkCBTInsertRemove(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt.Insert(i)
		if i%2 == 1 {
			cbt.RemoveLast()
		}
	}
}

func BenchmarkCBTSaveText(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 1000; i++ {
		cbt.Insert(i)
	}
	filename := "bench_cbt.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt.SaveText(filename)
	}
}

func BenchmarkCBTLoadText(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 1000; i++ {
		cbt.Insert(i)
	}
	filename := "bench_cbt.txt"
	cbt.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt2 := NewCompleteBinaryTree()
		cbt2.LoadText(filename)
	}
}

func BenchmarkCBTSaveBinary(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 1000; i++ {
		cbt.Insert(i)
	}
	filename := "bench_cbt.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt.SaveBinary(filename)
	}
}

func BenchmarkCBTLoadBinary(b *testing.B) {
	cbt := NewCompleteBinaryTree()
	for i := 0; i < 1000; i++ {
		cbt.Insert(i)
	}
	filename := "bench_cbt.bin"
	cbt.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		cbt2 := NewCompleteBinaryTree()
		cbt2.LoadBinary(filename)
	}
}
