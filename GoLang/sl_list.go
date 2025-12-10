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

type slistNode struct {
	value int
	next  *slistNode
}

type SinglyLinkedList struct {
	head *slistNode
	tail *slistNode
	size int
}

func NewSinglyLinkedList() *SinglyLinkedList {
	return &SinglyLinkedList{
		head: nil,
		tail: nil,
		size: 0,
	}
}

func (sl *SinglyLinkedList) Size() int {
	return sl.size
}

func (sl *SinglyLinkedList) IsEmpty() bool {
	return sl.size == 0
}

func (sl *SinglyLinkedList) AddHead(value int) {
	newNode := &slistNode{value: value, next: sl.head}
	sl.head = newNode
	
	if sl.tail == nil {
		sl.tail = newNode
	}
	
	sl.size++
}

func (sl *SinglyLinkedList) AddTail(value int) {
	newNode := &slistNode{value: value, next: nil}
	
	if sl.tail != nil {
		sl.tail.next = newNode
	} else {
		sl.head = newNode
	}
	
	sl.tail = newNode
	sl.size++
}

func (sl *SinglyLinkedList) RemoveHead() error {
	if sl.head == nil {
		return errors.New("list is empty")
	}
	
	sl.head = sl.head.next
	sl.size--
	
	if sl.head == nil {
		sl.tail = nil
	}
	
	return nil
}

func (sl *SinglyLinkedList) RemoveTail() error {
	if sl.head == nil {
		return errors.New("list is empty")
	}
	
	if sl.head == sl.tail {
		sl.head = nil
		sl.tail = nil
		sl.size = 0
		return nil
	}
	
	// Находим предпоследний узел
	current := sl.head
	for current.next != sl.tail {
		current = current.next
	}
	
	current.next = nil
	sl.tail = current
	sl.size--
	
	return nil
}

func (sl *SinglyLinkedList) Search(value int) int {
	current := sl.head
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

func (sl *SinglyLinkedList) Clear() {
	sl.head = nil
	sl.tail = nil
	sl.size = 0
}

func (sl *SinglyLinkedList) Print() {
	if sl.head == nil {
		fmt.Println("[]")
		return
	}
	
	fmt.Print("[")
	current := sl.head
	for current != nil {
		fmt.Print(current.value)
		if current.next != nil {
			fmt.Print(" -> ")
		}
		current = current.next
	}
	fmt.Println("]")
}

// SaveText saves list to text format
func (sl *SinglyLinkedList) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()
	
	fmt.Fprintf(file, "%d\n", sl.size)
	
	current := sl.head
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
func (sl *SinglyLinkedList) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()
	
	sl.Clear()
	
	var size int
	fmt.Fscanf(file, "%d\n", &size)
	
	if size > 0 {
		data, _ := io.ReadAll(file)
		parts := strings.Fields(string(data))
		
		for i := 0; i < size && i < len(parts); i++ {
			val, _ := strconv.Atoi(parts[i])
			sl.AddTail(val)
		}
	}
	
	return nil
}

// SaveBinary saves list to binary format
func (sl *SinglyLinkedList) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()
	
	binary.Write(file, binary.LittleEndian, int32(sl.size))
	
	current := sl.head
	for current != nil {
		binary.Write(file, binary.LittleEndian, int32(current.value))
		current = current.next
	}
	
	return nil
}

// LoadBinary loads list from binary format
func (sl *SinglyLinkedList) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()
	
	sl.Clear()
	
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
		sl.AddTail(int(val))
	}
	
	return nil
}
