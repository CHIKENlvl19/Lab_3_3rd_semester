package datastructures

import (
	"os"
	"testing"
)

func BenchmarkSListAddHead(b *testing.B) {
	sl := NewSinglyLinkedList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl.AddHead(i)
	}
}

func BenchmarkSListAddTail(b *testing.B) {
	sl := NewSinglyLinkedList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl.AddTail(i)
	}
}

func BenchmarkSListRemoveHead(b *testing.B) {
	// ИСПРАВЛЕНО: используем фиксированный размер
	sl := NewSinglyLinkedList()
	for i := 0; i < 10000; i++ {
		sl.AddTail(i)
	}
	
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		// Если список опустел, заполняем снова
		if sl.IsEmpty() {
			b.StopTimer()
			for j := 0; j < 10000; j++ {
				sl.AddTail(j)
			}
			b.StartTimer()
		}
		sl.RemoveHead()
	}
}

func BenchmarkSListRemoveTail(b *testing.B) {
	// ИСПРАВЛЕНО: упрощаем
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		// Если список опустел, заполняем снова
		if sl.IsEmpty() {
			b.StopTimer()
			for j := 0; j < 1000; j++ {
				sl.AddTail(j)
			}
			b.StartTimer()
		}
		sl.RemoveTail()
	}
}

func BenchmarkSListSearch(b *testing.B) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl.Search(i % 1000)
	}
}

func BenchmarkSListSaveText(b *testing.B) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	filename := "bench_slist.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl.SaveText(filename)
	}
}

func BenchmarkSListLoadText(b *testing.B) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	filename := "bench_slist.txt"
	sl.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl2 := NewSinglyLinkedList()
		sl2.LoadText(filename)
	}
}

func BenchmarkSListSaveBinary(b *testing.B) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	filename := "bench_slist.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl.SaveBinary(filename)
	}
}

func BenchmarkSListLoadBinary(b *testing.B) {
	sl := NewSinglyLinkedList()
	for i := 0; i < 1000; i++ {
		sl.AddTail(i)
	}
	filename := "bench_slist.bin"
	sl.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		sl2 := NewSinglyLinkedList()
		sl2.LoadBinary(filename)
	}
}
