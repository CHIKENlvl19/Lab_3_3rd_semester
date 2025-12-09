package datastructures

import (
	"os"
	"testing"
)

func BenchmarkDListAddHead(b *testing.B) {
	dl := NewDoublyLinkedList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl.AddHead(i)
	}
}

func BenchmarkDListAddTail(b *testing.B) {
	dl := NewDoublyLinkedList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl.AddTail(i)
	}
}

func BenchmarkDListSearch(b *testing.B) {
	dl := NewDoublyLinkedList()
	for i := 0; i < 1000; i++ {
		dl.AddTail(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl.Search(i % 1000)
	}
}

func BenchmarkDListRemoveByValue(b *testing.B) {
	b.StopTimer()
	for i := 0; i < b.N; i++ {
		dl := NewDoublyLinkedList()
		for j := 0; j < 1000; j++ {
			dl.AddTail(j)
		}
		b.StartTimer()
		dl.RemoveByValue(500)
		b.StopTimer()
	}
}

func BenchmarkDListSaveText(b *testing.B) {
	dl := NewDoublyLinkedList()
	for i := 0; i < 1000; i++ {
		dl.AddTail(i)
	}
	filename := "bench_dlist.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl.SaveText(filename)
	}
}

func BenchmarkDListLoadText(b *testing.B) {
	dl := NewDoublyLinkedList()
	for i := 0; i < 1000; i++ {
		dl.AddTail(i)
	}
	filename := "bench_dlist.txt"
	dl.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl2 := NewDoublyLinkedList()
		dl2.LoadText(filename)
	}
}
