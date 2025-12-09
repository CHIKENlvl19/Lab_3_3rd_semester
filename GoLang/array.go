package datastructures

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

var (
	ErrIndexOutOfRange = errors.New("index out of range")
	ErrCannotOpenFile  = errors.New("cannot open file")
)

type MyArray struct {
	data     []int
	size     int
	capacity int
}

func New(initialCapacity int) *MyArray {
	if initialCapacity <= 0 {
		initialCapacity = 4
	}
	return &MyArray{
		data:     make([]int, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
}

func (a *MyArray) Size() int {
	return a.size
}

func (a *MyArray) Capacity() int {
	return a.capacity
}

func (a *MyArray) Get(index int) (int, error) {
	if index < 0 || index >= a.size {
		return 0, ErrIndexOutOfRange
	}
	return a.data[index], nil
}

func (a *MyArray) Set(index int, value int) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}
	a.data[index] = value
	return nil
}

func (a *MyArray) PushBack(value int) {
	if a.size >= a.capacity {
		newCapacity := a.capacity * 2
		newData := make([]int, newCapacity)
		copy(newData, a.data[:a.size])
		a.data = newData
		a.capacity = newCapacity
	}
	a.data[a.size] = value
	a.size++
}

func (a *MyArray) Insert(index int, value int) error {
	if index < 0 || index > a.size {
		return ErrIndexOutOfRange
	}

	if a.size >= a.capacity {
		newCapacity := a.capacity * 2
		newData := make([]int, newCapacity)
		copy(newData, a.data[:a.size])
		a.data = newData
		a.capacity = newCapacity
	}

	// Shift elements
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.size++
	return nil
}

func (a *MyArray) Remove(index int) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}

	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	return nil
}

func (a *MyArray) Print() {
	fmt.Print("[")
	for i := 0; i < a.size; i++ {
		fmt.Print(a.data[i])
		if i < a.size-1 {
			fmt.Print(", ")
		}
	}
	fmt.Println("]")
}

// SaveText saves array to text format
func (a *MyArray) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return ErrCannotOpenFile
	}
	defer file.Close()

	fmt.Fprintf(file, "%d %d\n", a.size, a.capacity)
	for i := 0; i < a.size; i++ {
		fmt.Fprintf(file, "%d", a.data[i])
		if i < a.size-1 {
			fmt.Fprint(file, " ")
		}
	}
	return nil
}

// LoadText loads array from text format
func (a *MyArray) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return ErrCannotOpenFile
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		return errors.New("invalid file format")
	}

	parts := strings.Fields(scanner.Text())
	if len(parts) < 2 {
		return errors.New("invalid file format")
	}

	size, _ := strconv.Atoi(parts[0])
	capacity, _ := strconv.Atoi(parts[1])

	a.size = size
	a.capacity = capacity
	a.data = make([]int, capacity)

	if scanner.Scan() {
		parts = strings.Fields(scanner.Text())
		for i := 0; i < size && i < len(parts); i++ {
			a.data[i], _ = strconv.Atoi(parts[i])
		}
	}

	return nil
}

// SaveBinary saves array to binary format
func (a *MyArray) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return ErrCannotOpenFile
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(a.size))
	binary.Write(file, binary.LittleEndian, int32(a.capacity))

	for i := 0; i < a.size; i++ {
		binary.Write(file, binary.LittleEndian, int32(a.data[i]))
	}

	return nil
}

// LoadBinary loads array from binary format
func (a *MyArray) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return ErrCannotOpenFile
	}
	defer file.Close()

	var size, capacity int32
	binary.Read(file, binary.LittleEndian, &size)
	binary.Read(file, binary.LittleEndian, &capacity)

	a.size = int(size)
	a.capacity = int(capacity)
	a.data = make([]int, capacity)

	for i := 0; i < a.size; i++ {
		var val int32
		if err := binary.Read(file, binary.LittleEndian, &val); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		a.data[i] = int(val)
	}

	return nil
}
