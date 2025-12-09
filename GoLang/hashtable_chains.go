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

type chainNode struct {
	key   int
	value int
	next  *chainNode
}

type HashTableChains struct {
	buckets    []*chainNode
	size       int
	capacity   int
	loadFactor float64
	a, b, p    int
}

func NewHashTableChains(initialCapacity int) *HashTableChains {
	if initialCapacity <= 0 {
		initialCapacity = 16
	}

	return &HashTableChains{
		buckets:    make([]*chainNode, initialCapacity),
		size:       0,
		capacity:   initialCapacity,
		loadFactor: 0.0,
		a:          1,
		b:          0,
		p:          1000000007,
	}
}

func (ht *HashTableChains) hash(key int) int {
	return ((ht.a*key + ht.b) % ht.p) % ht.capacity
}

func (ht *HashTableChains) Size() int {
	return ht.size
}

func (ht *HashTableChains) Capacity() int {
	return ht.capacity
}

func (ht *HashTableChains) LoadFactor() float64 {
	return ht.loadFactor
}

func (ht *HashTableChains) resize() {
	oldBuckets := ht.buckets
	ht.capacity *= 2
	ht.buckets = make([]*chainNode, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0

	for _, head := range oldBuckets {
		current := head
		for current != nil {
			ht.Insert(current.key, current.value)
			current = current.next
		}
	}
}

func (ht *HashTableChains) Insert(key int, value int) {
	if ht.loadFactor >= 0.75 {
		ht.resize()
	}

	index := ht.hash(key)
	current := ht.buckets[index]

	// Проверяем, существует ли ключ (обновление)
	for current != nil {
		if current.key == key {
			current.value = value
			return
		}
		current = current.next
	}

	// Вставка нового узла в начало цепочки
	newNode := &chainNode{key: key, value: value, next: ht.buckets[index]}
	ht.buckets[index] = newNode
	ht.size++
	ht.loadFactor = float64(ht.size) / float64(ht.capacity)
}

func (ht *HashTableChains) Get(key int) (int, error) {
	index := ht.hash(key)
	current := ht.buckets[index]

	for current != nil {
		if current.key == key {
			return current.value, nil
		}
		current = current.next
	}

	return 0, errors.New("key not found")
}

func (ht *HashTableChains) Remove(key int) error {
	index := ht.hash(key)
	current := ht.buckets[index]
	var prev *chainNode

	for current != nil {
		if current.key == key {
			if prev == nil {
				ht.buckets[index] = current.next
			} else {
				prev.next = current.next
			}
			ht.size--
			ht.loadFactor = float64(ht.size) / float64(ht.capacity)
			return nil
		}
		prev = current
		current = current.next
	}

	return errors.New("key not found")
}

func (ht *HashTableChains) Contains(key int) bool {
	_, err := ht.Get(key)
	return err == nil
}

func (ht *HashTableChains) Clear() {
	ht.buckets = make([]*chainNode, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0
}

func (ht *HashTableChains) Print() {
	fmt.Println("HashTable (Chaining):")
	for i := 0; i < ht.capacity; i++ {
		if ht.buckets[i] != nil {
			fmt.Printf("  Bucket %d: ", i)
			current := ht.buckets[i]
			for current != nil {
				fmt.Printf("[%d->%d] ", current.key, current.value)
				current = current.next
			}
			fmt.Println()
		}
	}
}

// SaveText saves hash table to text format
func (ht *HashTableChains) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	fmt.Fprintf(file, "%d %d %d %d %d\n", ht.size, ht.capacity, ht.a, ht.b, ht.p)

	for i := 0; i < ht.capacity; i++ {
		current := ht.buckets[i]
		for current != nil {
			fmt.Fprintf(file, "%d %d\n", current.key, current.value)
			current = current.next
		}
	}

	return nil
}

// LoadText loads hash table from text format
func (ht *HashTableChains) LoadText(filename string) error {
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
	if len(parts) < 5 {
		return errors.New("invalid file format")
	}

	oldSize, _ := strconv.Atoi(parts[0])
	ht.capacity, _ = strconv.Atoi(parts[1])
	ht.a, _ = strconv.Atoi(parts[2])
	ht.b, _ = strconv.Atoi(parts[3])
	ht.p, _ = strconv.Atoi(parts[4])

	ht.buckets = make([]*chainNode, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0

	for i := 0; i < oldSize; i++ {
		if !scanner.Scan() {
			break
		}
		parts = strings.Fields(scanner.Text())
		if len(parts) < 2 {
			continue
		}
		key, _ := strconv.Atoi(parts[0])
		value, _ := strconv.Atoi(parts[1])
		ht.Insert(key, value)
	}

	return nil
}

// SaveBinary saves hash table to binary format
func (ht *HashTableChains) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(ht.size))
	binary.Write(file, binary.LittleEndian, int32(ht.capacity))
	binary.Write(file, binary.LittleEndian, int32(ht.a))
	binary.Write(file, binary.LittleEndian, int32(ht.b))
	binary.Write(file, binary.LittleEndian, int32(ht.p))

	for i := 0; i < ht.capacity; i++ {
		current := ht.buckets[i]
		for current != nil {
			binary.Write(file, binary.LittleEndian, int32(current.key))
			binary.Write(file, binary.LittleEndian, int32(current.value))
			current = current.next
		}
	}

	return nil
}

// LoadBinary loads hash table from binary format
func (ht *HashTableChains) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	var oldSize, capacity, a, b, p int32
	binary.Read(file, binary.LittleEndian, &oldSize)
	binary.Read(file, binary.LittleEndian, &capacity)
	binary.Read(file, binary.LittleEndian, &a)
	binary.Read(file, binary.LittleEndian, &b)
	binary.Read(file, binary.LittleEndian, &p)

	ht.capacity = int(capacity)
	ht.a = int(a)
	ht.b = int(b)
	ht.p = int(p)
	ht.buckets = make([]*chainNode, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0

	for i := 0; i < int(oldSize); i++ {
		var key, value int32
		if err := binary.Read(file, binary.LittleEndian, &key); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		if err := binary.Read(file, binary.LittleEndian, &value); err != nil {
			if err == io.EOF {
				break
			}
			return err
		}
		ht.Insert(int(key), int(value))
	}

	return nil
}
