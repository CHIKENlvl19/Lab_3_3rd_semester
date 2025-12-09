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

type cbtNode struct {
	value int
	left  *cbtNode
	right *cbtNode
}

type CompleteBinaryTree struct {
	root *cbtNode
	size int
}

func NewCompleteBinaryTree() *CompleteBinaryTree {
	return &CompleteBinaryTree{
		root: nil,
		size: 0,
	}
}

func (cbt *CompleteBinaryTree) Size() int {
	return cbt.size
}

func (cbt *CompleteBinaryTree) IsEmpty() bool {
	return cbt.size == 0
}

func (cbt *CompleteBinaryTree) GetRoot() (int, error) {
	if cbt.root == nil {
		return 0, errors.New("tree is empty")
	}
	return cbt.root.value, nil
}

// findParent находит родителя для узла по индексу
func (cbt *CompleteBinaryTree) findParent(index int) (*cbtNode, bool) {
	if index <= 1 {
		return nil, false
	}

	path := index
	depth := 0

	for path > 1 {
		path >>= 1
		depth++
	}

	current := cbt.root
	for level := depth - 1; level > 0; level-- {
		if index&(1<<level) != 0 {
			current = current.right
		} else {
			current = current.left
		}
	}

	goRight := (index & 1) != 0
	return current, goRight
}

func (cbt *CompleteBinaryTree) Insert(value int) {
	newNode := &cbtNode{value: value, left: nil, right: nil}
	cbt.size++

	if cbt.root == nil {
		cbt.root = newNode
		return
	}

	parent, goRight := cbt.findParent(cbt.size)

	if goRight {
		parent.right = newNode
	} else {
		parent.left = newNode
	}
}

func (cbt *CompleteBinaryTree) RemoveLast() error {
	if cbt.root == nil {
		return errors.New("tree is empty")
	}

	if cbt.size == 1 {
		cbt.root = nil
		cbt.size = 0
		return nil
	}

	parent, goRight := cbt.findParent(cbt.size)

	if goRight {
		parent.right = nil
	} else {
		parent.left = nil
	}

	cbt.size--
	return nil
}

func (cbt *CompleteBinaryTree) Clear() {
	cbt.root = nil
	cbt.size = 0
}

func (cbt *CompleteBinaryTree) PrintLevelOrder() {
	if cbt.root == nil {
		fmt.Println("[]")
		return
	}

	queue := []*cbtNode{cbt.root}
	fmt.Print("[")
	first := true

	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]

		if !first {
			fmt.Print(", ")
		}
		first = false
		fmt.Print(node.value)

		if node.left != nil {
			queue = append(queue, node.left)
		}
		if node.right != nil {
			queue = append(queue, node.right)
		}
	}
	fmt.Println("]")
}

// SaveText saves tree to text format
func (cbt *CompleteBinaryTree) SaveText(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	fmt.Fprintf(file, "%d\n", cbt.size)

	if cbt.root != nil {
		queue := []*cbtNode{cbt.root}
		first := true

		for len(queue) > 0 {
			node := queue[0]
			queue = queue[1:]

			if !first {
				fmt.Fprint(file, " ")
			}
			first = false
			fmt.Fprintf(file, "%d", node.value)

			if node.left != nil {
				queue = append(queue, node.left)
			}
			if node.right != nil {
				queue = append(queue, node.right)
			}
		}
	}

	return nil
}

// LoadText loads tree from text format
func (cbt *CompleteBinaryTree) LoadText(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	cbt.Clear()

	var size int
	fmt.Fscanf(file, "%d\n", &size)

	if size > 0 {
		data, _ := io.ReadAll(file)
		parts := strings.Fields(string(data))

		for i := 0; i < size && i < len(parts); i++ {
			val, _ := strconv.Atoi(parts[i])
			cbt.Insert(val)
		}
	}

	return nil
}

// SaveBinary saves tree to binary format
func (cbt *CompleteBinaryTree) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("cannot open file for writing")
	}
	defer file.Close()

	binary.Write(file, binary.LittleEndian, int32(cbt.size))

	if cbt.root != nil {
		queue := []*cbtNode{cbt.root}

		for len(queue) > 0 {
			node := queue[0]
			queue = queue[1:]

			binary.Write(file, binary.LittleEndian, int32(node.value))

			if node.left != nil {
				queue = append(queue, node.left)
			}
			if node.right != nil {
				queue = append(queue, node.right)
			}
		}
	}

	return nil
}

// LoadBinary loads tree from binary format
func (cbt *CompleteBinaryTree) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("cannot open file for reading")
	}
	defer file.Close()

	cbt.Clear()

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
		cbt.Insert(int(val))
	}

	return nil
}
