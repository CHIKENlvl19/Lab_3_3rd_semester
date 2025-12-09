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

const (
	CellEmpty    = 0
	CellOccupied = 1
	CellDeleted  = 2
)

type oaCell struct {
	key   int
	value int
	state int
}

type HashTableOA struct {
	table      []oaCell
	size       int
	capacity   int
	loadFactor float64
	a, b, p    int
}

func NewHashTableOA(initialCapacity int) *HashTableOA {
	if initialCapacity <= 0 {
		initialCapacity = 16
	}

	return &HashTableOA{
		table:      make([]oaCell, initialCapacity),
		size:       0,
		capacity:   initialCapacity,
		loadFactor: 0.0,
		a:          1,
		b:          0,
		p:          1000000007,
	}
}

func (ht *HashTableOA) hash(key int, attempt int) int {
	h1 := ((ht.a*key + ht.b) % ht.p) % ht.capacity
	h2 := 1 + (key % (ht.capacity - 1))
	return (h1 + attempt*h2) % ht.capacity
}

func (ht *HashTableOA) Size() int {
	return ht.size
}

func (ht *HashTableOA) Capacity() int {
	return ht.capacity
}

func (ht *HashTableOA) LoadFactor() float64 {
	return ht.loadFactor
}

func (ht *HashTableOA) resize() {
	oldTable := ht.table
	ht.capacity *= 2
	ht.table = make([]oaCell, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0

	for i := range oldTable {
		if oldTable[i].state == CellOccupied {
			ht.Insert(oldTable[i].key, oldTable[i].value)
		}
	}
}

func (ht *HashTableOA) Insert(key int, value int) {
	if ht.loadFactor >= 0.7 {
		ht.resize()
	}

	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)

		if ht.table[index].state == CellEmpty || ht.table[index].state == CellDeleted {
			ht.table[index].key = key
			ht.table[index].value = value
			ht.table[index].state = CellOccupied
			ht.size++
			ht.loadFactor = float64(ht.size) / float64(ht.capacity)
			return
		}

		if ht.table[index].state == CellOccupied && ht.table[index].key == key {
			ht.table[index].value = value
			return
		}
	}
}

func (ht *HashTableOA) Get(key int) (int, error) {
	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)

		if ht.table[index].state == CellEmpty {
			return 0, errors.New("key not found")
		}

		if ht.table[index].state == CellOccupied && ht.table[index].key == key {
			return ht.table[index].value, nil
		}
	}

	return 0, errors.New("key not found")
}

func (ht *HashTableOA) Remove(key int) error {
	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)

		if ht.table[index].state == CellEmpty {
			return errors.New("key not found")
		}

		if ht.table[index].state == CellOccupied && ht.table[index].key == key {
			ht.table[index].state = CellDeleted
			ht.size--
			ht.loadFactor = float64(ht.size) / float64(ht.capacity)
			return nil
		}
	}

	return errors.New("key not found")
}

func (ht *HashTableOA) Contains(key int) bool {
	_, err := ht.Get(key)
	return err == nil
}

func (ht *HashTableOA) Clear() {
	ht.table = make([]oaCell, ht.capacity)
	ht.size = 0
	ht.loadFactor = 0.0
}

func (ht *HashTableOA) Print() {
	fmt.Println("HashTable (Open Addressing):")
	for i := 0; i < ht.capacity; i++ {
		if ht.table[i].state == CellOccupied {
			fmt.Printf("  [%d]: %d -> %d\n", i, ht.table[i].key, ht.table[i].value)
		}
	}
}

// SaveText saves hash table to text format
func (ht *HashTableOA) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	fmt.Fprintf(file, "%d %d %d %d %d\n", ht.size, ht.capacity, ht.a, ht.b, ht.p)

	for i := 0; i < ht.capacity; i++ {
		if ht.table[i].state == CellOccupied {
			fmt.Fprintf(file, "%d %d\n", ht.table[i].key, ht.table[i].value)
		}
	}

	return nil
}

// LoadText loads hash table from text format
func (ht *HashTableOA) LoadText(filename string) error {
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

	ht.table = make([]oaCell, ht.capacity)
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
func (ht *HashTableOA) SaveBinary(filename string) error {
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
		if ht.table[i].state == CellOccupied {
			binary.Write(file, binary.LittleEndian, int32(ht.table[i].key))
			binary.Write(file, binary.LittleEndian, int32(ht.table[i].value))
		}
	}

	return nil
}

// LoadBinary loads hash table from binary format
func (ht *HashTableOA) LoadBinary(filename string) error {
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
	ht.table = make([]oaCell, ht.capacity)
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
