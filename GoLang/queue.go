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

type MyQueue struct {
	data []int
	head int
}

func NewQueue(initialCapacity int) *MyQueue {
	if initialCapacity <= 0 {
		initialCapacity = 4
	}
	return &MyQueue{
		data: make([]int, 0, initialCapacity),
		head: 0,
	}
}

func (q *MyQueue) Size() int {
	return len(q.data) - q.head
}

func (q *MyQueue) Capacity() int {
	return cap(q.data)
}

func (q *MyQueue) IsEmpty() bool {
	return len(q.data) == q.head
}

func (q *MyQueue) Push(value int) {
	q.data = append(q.data, value)
}

func (q *MyQueue) Pop() error {
	if q.IsEmpty() {
		return errors.New("queue is empty")
	}

	q.head++

	// Оптимизация: если queue стал слишком разреженным, пересоздаем
	if q.head > len(q.data)/2 && q.head > 10 {
		q.compact()
	}

	return nil
}

func (q *MyQueue) Front() (int, error) {
	if q.IsEmpty() {
		return 0, errors.New("queue is empty")
	}
	return q.data[q.head], nil
}

// compact уплотняет данные, удаляя пустое пространство в начале
func (q *MyQueue) compact() {
	newData := make([]int, 0, cap(q.data))
	newData = append(newData, q.data[q.head:]...)
	q.data = newData
	q.head = 0
}

func (q *MyQueue) Clear() {
	q.data = q.data[:0]
	q.head = 0
}

func (q *MyQueue) Print() {
	if q.IsEmpty() {
		fmt.Println("Queue is empty!")
		return
	}
	fmt.Print("[ ")
	for i := q.head; i < len(q.data); i++ {
		fmt.Print(q.data[i], " ")
	}
	fmt.Println("]")
}

// SaveText saves queue to text format
func (q *MyQueue) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	size := q.Size()
	fmt.Fprintf(file, "%d %d\n", size, cap(q.data))

	for i := q.head; i < len(q.data); i++ {
		fmt.Fprintf(file, "%d", q.data[i])
		if i < len(q.data)-1 {
			fmt.Fprint(file, " ")
		}
	}

	return nil
}

// LoadText loads queue from text format
func (q *MyQueue) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
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

	q.data = make([]int, size, capacity)
	q.head = 0

	if scanner.Scan() {
		parts = strings.Fields(scanner.Text())
		for i := 0; i < size && i < len(parts); i++ {
			q.data[i], _ = strconv.Atoi(parts[i])
		}
	}

	return nil
}

// SaveBinary saves queue to binary format
func (q *MyQueue) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	size := q.Size()
	binary.Write(file, binary.LittleEndian, int32(size))
	binary.Write(file, binary.LittleEndian, int32(cap(q.data)))

	for i := q.head; i < len(q.data); i++ {
		binary.Write(file, binary.LittleEndian, int32(q.data[i]))
	}

	return nil
}

// LoadBinary loads queue from binary format
func (q *MyQueue) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	var size, capacity int32
	binary.Read(file, binary.LittleEndian, &size)
	binary.Read(file, binary.LittleEndian, &capacity)

	q.data = make([]int, size, capacity)
	q.head = 0

	for i := 0; i < int(size); i++ {
		var val int32
		if err := binary.Read(file, binary.LittleEndian, &val); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		q.data[i] = int(val)
	}

	return nil
}
