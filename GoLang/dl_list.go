package datastructures

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

type dlistNode struct {
	value int
	next  *dlistNode
	prev  *dlistNode
}

type DoublyLinkedList struct {
	head *dlistNode
	tail *dlistNode
	size int
}

func NewDoublyLinkedList() *DoublyLinkedList {
	return &DoublyLinkedList{
		head: nil,
		tail: nil,
		size: 0,
	}
}

func (dl *DoublyLinkedList) Size() int {
	return dl.size
}

func (dl *DoublyLinkedList) IsEmpty() bool {
	return dl.size == 0
}

func (dl *DoublyLinkedList) AddHead(value int) {
	newNode := &dlistNode{value: value, next: dl.head, prev: nil}

	if dl.head != nil {
		dl.head.prev = newNode
	} else {
		dl.tail = newNode
	}

	dl.head = newNode
	dl.size++
}

func (dl *DoublyLinkedList) AddTail(value int) {
	newNode := &dlistNode{value: value, next: nil, prev: dl.tail}

	if dl.tail != nil {
		dl.tail.next = newNode
	} else {
		dl.head = newNode
	}

	dl.tail = newNode
	dl.size++
}

func (dl *DoublyLinkedList) AddAfter(index int, value int) error {
	if index < 0 || index > dl.size {
		return errors.New("index out of range")
	}

	if index == dl.size {
		dl.AddTail(value)
		return nil
	}

	current := dl.head
	for i := 0; i < index; i++ {
		current = current.next
	}

	newNode := &dlistNode{value: value, next: current.next, prev: current}

	if current.next != nil {
		current.next.prev = newNode
	} else {
		dl.tail = newNode
	}

	current.next = newNode
	dl.size++

	return nil
}

func (dl *DoublyLinkedList) RemoveByValue(value int) bool {
	current := dl.head

	for current != nil {
		if current.value == value {
			if current.prev != nil {
				current.prev.next = current.next
			} else {
				dl.head = current.next
			}

			if current.next != nil {
				current.next.prev = current.prev
			} else {
				dl.tail = current.prev
			}

			dl.size--
			return true
		}
		current = current.next
	}

	return false
}

func (dl *DoublyLinkedList) Search(value int) int {
	current := dl.head
	index := 0

	for current != nil {
		if current.value == value {
			return index
		}
		current = current.next
		index++
	}

	return -1
}

func (dl *DoublyLinkedList) Clear() {
	dl.head = nil
	dl.tail = nil
	dl.size = 0
}

func (dl *DoublyLinkedList) Print() {
	if dl.head == nil {
		fmt.Println("[]")
		return
	}

	fmt.Print("[")
	current := dl.head
	for current != nil {
		fmt.Print(current.value)
		if current.next != nil {
			fmt.Print(" <-> ")
		}
		current = current.next
	}
	fmt.Println("]")
}

func (dl *DoublyLinkedList) PrintBackwards() {
	if dl.tail == nil {
		fmt.Println("[]")
		return
	}

	fmt.Print("[")
	current := dl.tail
	for current != nil {
		fmt.Print(current.value)
		if current.prev != nil {
			fmt.Print(" <-> ")
		}
		current = current.prev
	}
	fmt.Println("]")
}

// SaveText saves list to text format
func (dl *DoublyLinkedList) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	fmt.Fprintf(file, "%d\n", dl.size)

	current := dl.head
	for current != nil {
		fmt.Fprintf(file, "%d", current.value)
		if current.next != nil {
			fmt.Fprint(file, " ")
		}
		current = current.next
	}

	return nil
}

// LoadText loads list from text format
func (dl *DoublyLinkedList) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	dl.Clear()

	var size int
	fmt.Fscanf(file, "%d\n", &size)

	if size > 0 {
		data, _ := io.ReadAll(file)
		parts := strings.Fields(string(data))

		for i := 0; i < size && i < len(parts); i++ {
			val, _ := strconv.Atoi(parts[i])
			dl.AddTail(val)
		}
	}

	return nil
}

// SaveBinary saves list to binary format
func (dl *DoublyLinkedList) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(dl.size))

	current := dl.head
	for current != nil {
		binary.Write(file, binary.LittleEndian, int32(current.value))
		current = current.next
	}

	return nil
}

// LoadBinary loads list from binary format
func (dl *DoublyLinkedList) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	dl.Clear()

	var size int32
	binary.Read(file, binary.LittleEndian, &size)

	for i := 0; i < int(size); i++ {
		var val int32
		if err := binary.Read(file, binary.LittleEndian, &val); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		dl.AddTail(int(val))
	}

	return nil
}
