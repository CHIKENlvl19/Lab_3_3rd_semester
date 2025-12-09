package datastructures

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
)

const MaxStackSize = 1000000

type MyStack struct {
	data []int
	size int
}

func NewStack() *MyStack {
	return &MyStack{
		data: make([]int, 0, 16), // Начальная capacity 16
		size: 0,
	}
}

func (s *MyStack) Size() int {
	return s.size
}

func (s *MyStack) IsEmpty() bool {
	return s.size == 0
}

func (s *MyStack) Push(value int) error {
	if s.size >= MaxStackSize {
		return errors.New("stack overflow")
	}
	s.data = append(s.data, value)
	s.size++
	return nil
}

func (s *MyStack) Pop() error {
	if s.size == 0 {
		return errors.New("stack is empty")
	}
	s.data = s.data[:s.size-1]
	s.size--
	return nil
}

func (s *MyStack) Peek() (int, error) {
	if s.size == 0 {
		return 0, errors.New("stack is empty")
	}
	return s.data[s.size-1], nil
}

func (s *MyStack) Clear() {
	s.data = s.data[:0]
	s.size = 0
}

func (s *MyStack) Print() {
	if s.size == 0 {
		fmt.Println("Stack is empty!")
		return
	}
	fmt.Print("nil")
	for i := s.size - 1; i >= 0; i-- {
		fmt.Printf(" <- %d", s.data[i])
	}
	fmt.Println()
}

// SaveText saves stack to text format
func (s *MyStack) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	fmt.Fprintf(file, "%d\n", s.size)

	for i := 0; i < s.size; i++ {
		fmt.Fprintf(file, "%d", s.data[i])
		if i < s.size-1 {
			fmt.Fprint(file, " ")
		}
	}

	return nil
}

// LoadText loads stack from text format
func (s *MyStack) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	s.Clear()

	var size int
	fmt.Fscanf(file, "%d\n", &size)

	s.data = make([]int, size)
	for i := 0; i < size; i++ {
		fmt.Fscanf(file, "%d", &s.data[i])
	}
	s.size = size

	return nil
}

// SaveBinary saves stack to binary format
func (s *MyStack) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(s.size))

	for i := 0; i < s.size; i++ {
		binary.Write(file, binary.LittleEndian, int32(s.data[i]))
	}

	return nil
}

// LoadBinary loads stack from binary format
func (s *MyStack) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	var size int32
	binary.Read(file, binary.LittleEndian, &size)

	s.data = make([]int, size)
	for i := 0; i < int(size); i++ {
		var val int32
		if err := binary.Read(file, binary.LittleEndian, &val); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		s.data[i] = int(val)
	}
	s.size = int(size)

	return nil
}
