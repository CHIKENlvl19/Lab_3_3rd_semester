package datastructures

import (
	"os"
	"testing"
)

func BenchmarkHashTableOAInsert(b *testing.B) {
	ht := NewHashTableOA(16)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Insert(i, i*10)
	}
}

func BenchmarkHashTableOAGet(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Get(i % 1000)
	}
}

func BenchmarkHashTableOARemove(b *testing.B) {
	// ФИКСИРОВАННЫЙ размер - удаляем циклически
	ht := NewHashTableOA(10000)
	for i := 0; i < 10000; i++ {
		ht.Insert(i, i*10)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		key := i % 10000
		ht.Remove(key)
		// Добавляем обратно чтобы таблица не опустела
		ht.Insert(key, key*10)
	}
}

func BenchmarkHashTableOAContains(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Contains(i % 1000)
	}
}

func BenchmarkHashTableOASaveText(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_oa.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.SaveText(filename)
	}
}

func BenchmarkHashTableOALoadText(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_oa.txt"
	ht.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht2 := NewHashTableOA(16)
		ht2.LoadText(filename)
	}
}

func BenchmarkHashTableOASaveBinary(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_oa.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.SaveBinary(filename)
	}
}

func BenchmarkHashTableOALoadBinary(b *testing.B) {
	ht := NewHashTableOA(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_oa.bin"
	ht.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht2 := NewHashTableOA(16)
		ht2.LoadBinary(filename)
	}
}
