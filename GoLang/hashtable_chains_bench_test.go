package datastructures

import (
	"os"
	"testing"
)

func BenchmarkHashTableChainsInsert(b *testing.B) {
	ht := NewHashTableChains(16)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Insert(i, i*10)
	}
}

func BenchmarkHashTableChainsGet(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Get(i % 1000)
	}
}

func BenchmarkHashTableChainsRemove(b *testing.B) {
	// ФИКСИРОВАННЫЙ размер - удаляем циклически
	ht := NewHashTableChains(10000)
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

func BenchmarkHashTableChainsContains(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Contains(i % 1000)
	}
}

func BenchmarkHashTableChainsSaveText(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_chains.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.SaveText(filename)
	}
}

func BenchmarkHashTableChainsLoadText(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_chains.txt"
	ht.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht2 := NewHashTableChains(16)
		ht2.LoadText(filename)
	}
}

func BenchmarkHashTableChainsSaveBinary(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_chains.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.SaveBinary(filename)
	}
}

func BenchmarkHashTableChainsLoadBinary(b *testing.B) {
	ht := NewHashTableChains(1000)
	for i := 0; i < 1000; i++ {
		ht.Insert(i, i*10)
	}
	filename := "bench_ht_chains.bin"
	ht.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht2 := NewHashTableChains(16)
		ht2.LoadBinary(filename)
	}
}
