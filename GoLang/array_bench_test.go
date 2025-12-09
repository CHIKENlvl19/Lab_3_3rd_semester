package datastructures

import (
	"os"
	"testing"
)

func BenchmarkPushBack(b *testing.B) {
	arr := New(4)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.PushBack(i)
	}
}

func BenchmarkInsertMiddle(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.Insert(500, i)
	}
}

func BenchmarkGet(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.Get(i % 1000)
	}
}

func BenchmarkSaveText(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	filename := "bench_array.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.SaveText(filename)
	}
}

func BenchmarkLoadText(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	filename := "bench_array.txt"
	arr.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr2 := New(4)
		arr2.LoadText(filename)
	}
}

func BenchmarkSaveBinary(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	filename := "bench_array.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.SaveBinary(filename)
	}
}

func BenchmarkLoadBinary(b *testing.B) {
	arr := New(1000)
	for i := 0; i < 1000; i++ {
		arr.PushBack(i)
	}
	filename := "bench_array.bin"
	arr.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr2 := New(4)
		arr2.LoadBinary(filename)
	}
}
