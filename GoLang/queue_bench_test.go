package datastructures

import (
	"os"
	"testing"
)

func BenchmarkQueuePush(b *testing.B) {
	q := NewQueue(4)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.Push(i)
	}
}

func BenchmarkQueuePop(b *testing.B) {
	q := NewQueue(b.N)
	for i := 0; i < b.N; i++ {
		q.Push(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.Pop()
	}
}

func BenchmarkQueueSaveText(b *testing.B) {
	q := NewQueue(1000)
	for i := 0; i < 1000; i++ {
		q.Push(i)
	}
	filename := "bench_queue.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.SaveText(filename)
	}
}

func BenchmarkQueueLoadText(b *testing.B) {
	q := NewQueue(1000)
	for i := 0; i < 1000; i++ {
		q.Push(i)
	}
	filename := "bench_queue.txt"
	q.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q2 := NewQueue(4)
		q2.LoadText(filename)
	}
}

func BenchmarkQueueSaveBinary(b *testing.B) {
	q := NewQueue(1000)
	for i := 0; i < 1000; i++ {
		q.Push(i)
	}
	filename := "bench_queue.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.SaveBinary(filename)
	}
}

func BenchmarkQueueLoadBinary(b *testing.B) {
	q := NewQueue(1000)
	for i := 0; i < 1000; i++ {
		q.Push(i)
	}
	filename := "bench_queue.bin"
	q.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q2 := NewQueue(4)
		q2.LoadBinary(filename)
	}
}
