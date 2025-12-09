package datastructures

import (
	"os"
	"testing"
)

func BenchmarkStackPush(b *testing.B) {
	s := NewStack()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.Push(i)
	}
}

func BenchmarkStackPop(b *testing.B) {
	s := NewStack()
	for i := 0; i < b.N; i++ {
		s.Push(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.Pop()
	}
}

func BenchmarkStackPeek(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.Peek()
	}
}

func BenchmarkStackPushPop(b *testing.B) {
	s := NewStack()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.Push(i)
		s.Pop()
	}
}

func BenchmarkStackSaveText(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	filename := "bench_stack.txt"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.SaveText(filename)
	}
}

func BenchmarkStackLoadText(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	filename := "bench_stack.txt"
	s.SaveText(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s2 := NewStack()
		s2.LoadText(filename)
	}
}

func BenchmarkStackSaveBinary(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	filename := "bench_stack.bin"
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.SaveBinary(filename)
	}
}

func BenchmarkStackLoadBinary(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	filename := "bench_stack.bin"
	s.SaveBinary(filename)
	defer os.Remove(filename)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s2 := NewStack()
		s2.LoadBinary(filename)
	}
}

func BenchmarkStackClear(b *testing.B) {
	s := NewStack()
	for i := 0; i < 1000; i++ {
		s.Push(i)
	}
	
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		// очищаем и заполняем снова
		s.Clear()
		for j := 0; j < 1000; j++ {
			s.Push(j)
		}
	}
}

