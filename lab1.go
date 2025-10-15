package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

// ==================== ПЕРЕЧИСЛЕНИЯ И СТРУКТУРЫ ====================

type Color int

const (
	RED Color = iota
	BLACK
)

type RBNode struct {
	data   int
	color  Color
	parent *RBNode
	left   *RBNode
	right  *RBNode
}

type RBTree struct {
	root *RBNode
}

type SNode struct {
	data int
	next *SNode
}

type Stack struct {
	head *SNode
}

type QNode struct {
	data     int
	previous *QNode
	next     *QNode
}

type Queue struct {
	tail *QNode
	head *QNode
}

type LLNode struct {
	data     int
	previous *LLNode
	next     *LLNode
}

type LList struct {
	tail *LLNode
	head *LLNode
}

type FLNode struct {
	data int
	next *FLNode
}

type FList struct {
	head *FLNode
}

type Array struct {
	data []int
}

type Database struct {
	arrays   map[string]*Array
	flists   map[string]*FList
	llists   map[string]*LList
	queues   map[string]*Queue
	stacks   map[string]*Stack
	trees    map[string]*RBTree
	filename string
}

// ==================== КРАСНО-ЧЕРНЫЕ ДЕРЕВЬЯ ====================

func leftRotate(tree *RBTree, x *RBNode) {
	y := x.right
	x.right = y.left
	if y.left != nil {
		y.left.parent = x
	}
	y.parent = x.parent
	if x.parent == nil {
		tree.root = y
	} else {
		if x == x.parent.left {
			x.parent.left = y
		} else {
			x.parent.right = y
		}
	}
	y.left = x
	x.parent = y
}

func rightRotate(tree *RBTree, x *RBNode) {
	y := x.left
	x.left = y.right
	if y.right != nil {
		y.right.parent = x
	}
	y.parent = x.parent
	if x.parent == nil {
		tree.root = y
	} else {
		if x == x.parent.left {
			x.parent.left = y
		} else {
			x.parent.right = y
		}
	}
	y.right = x
	x.parent = y
}

func fixAdd(tree *RBTree, z *RBNode) {
	for z.parent != nil && z.parent.color == RED {
		if z.parent.parent == nil {
			break
		}

		if z.parent == z.parent.parent.left {
			y := z.parent.parent.right

			if y != nil && y.color == RED {
				z.parent.color = BLACK
				y.color = BLACK
				z.parent.parent.color = RED
				z = z.parent.parent
			} else {
				if z == z.parent.right {
					z = z.parent
					leftRotate(tree, z)
				}
				z.parent.color = BLACK
				z.parent.parent.color = RED
				rightRotate(tree, z.parent.parent)
			}
		} else {
			y := z.parent.parent.left

			if y != nil && y.color == RED {
				z.parent.color = BLACK
				y.color = BLACK
				z.parent.parent.color = RED
				z = z.parent.parent
			} else {
				if z == z.parent.left {
					z = z.parent
					rightRotate(tree, z)
				}
				z.parent.color = BLACK
				z.parent.parent.color = RED
				leftRotate(tree, z.parent.parent)
			}
		}

		if z == tree.root {
			break
		}
	}
	tree.root.color = BLACK
}

func addRBNode(tree *RBTree, value int) {
	newRBNode := &RBNode{data: value, color: RED}
	if tree.root == nil {
		newRBNode.color = BLACK
		tree.root = newRBNode
		return
	}

	address := tree.root
	for {
		if value < address.data {
			if address.left == nil {
				newRBNode.parent = address
				address.left = newRBNode
				fixAdd(tree, newRBNode)
				return
			} else {
				address = address.left
			}
		} else {
			if address.right == nil {
				newRBNode.parent = address
				address.right = newRBNode
				fixAdd(tree, newRBNode)
				return
			} else {
				address = address.right
			}
		}
	}
}

func getRBNode(tree *RBTree, value int) *RBNode {
	if tree.root == nil {
		fmt.Println("Дерево пустое")
		return nil
	}

	address := tree.root
	for address != nil {
		if value == address.data {
			return address
		} else if value < address.data {
			address = address.left
		} else {
			address = address.right
		}
	}

	fmt.Printf("Узел %d отсутствует\n", value)
	return nil
}

func treeMinimum(tree *RBTree, node *RBNode) *RBNode {
	if node == nil {
		return nil
	}
	address := node
	for address.left != nil {
		address = address.left
	}
	return address
}

func deleteFix(tree *RBTree, x *RBNode) {
	for x != tree.root && (x == nil || x.color == BLACK) {
		if x == nil {
			break
		}

		if x == x.parent.left {
			w := x.parent.right
			if w == nil {
				break
			}

			if w.color == RED {
				w.color = BLACK
				x.parent.color = RED
				leftRotate(tree, x.parent)
				w = x.parent.right
				if w == nil {
					break
				}
			}

			leftBlack := (w.left == nil) || (w.left.color == BLACK)
			rightBlack := (w.right == nil) || (w.right.color == BLACK)

			if leftBlack && rightBlack {
				w.color = RED
				x = x.parent
			} else {
				if w.right == nil || w.right.color == BLACK {
					if w.left != nil {
						w.left.color = BLACK
					}
					w.color = RED
					rightRotate(tree, w)
					w = x.parent.right
					if w == nil {
						break
					}
				}

				w.color = x.parent.color
				x.parent.color = BLACK
				if w.right != nil {
					w.right.color = BLACK
				}
				leftRotate(tree, x.parent)
				x = tree.root
			}
		} else {
			w := x.parent.left
			if w == nil {
				break
			}

			if w.color == RED {
				w.color = BLACK
				x.parent.color = RED
				rightRotate(tree, x.parent)
				w = x.parent.left
				if w == nil {
					break
				}
			}

			leftBlack := (w.left == nil) || (w.left.color == BLACK)
			rightBlack := (w.right == nil) || (w.right.color == BLACK)

			if leftBlack && rightBlack {
				w.color = RED
				x = x.parent
			} else {
				if w.left == nil || w.left.color == BLACK {
					if w.right != nil {
						w.right.color = BLACK
					}
					w.color = RED
					leftRotate(tree, w)
					w = x.parent.left
					if w == nil {
						break
					}
				}

				w.color = x.parent.color
				x.parent.color = BLACK
				if w.left != nil {
					w.left.color = BLACK
				}
				rightRotate(tree, x.parent)
				x = tree.root
			}
		}
	}

	if x != nil {
		x.color = BLACK
	}
}

func transplant(tree *RBTree, u *RBNode, v *RBNode) {
	if u.parent == nil {
		tree.root = v
	} else if u == u.parent.left {
		u.parent.left = v
	} else {
		u.parent.right = v
	}

	if v != nil {
		v.parent = u.parent
	}
}

func delRBNode(tree *RBTree, value int) {
	z := getRBNode(tree, value)
	if z == nil {
		return
	}

	y := z
	var x *RBNode
	yOriginalColor := y.color

	if z.left == nil {
		x = z.right
		if x == nil {
			x = &RBNode{data: -1, color: BLACK, parent: z.parent}
		}
		transplant(tree, z, x)
	} else if z.right == nil {
		x = z.left
		if x == nil {
			x = &RBNode{data: -1, color: BLACK, parent: z.parent}
		}
		transplant(tree, z, x)
	} else {
		y = treeMinimum(tree, z.right)
		yOriginalColor = y.color
		x = y.right

		if x == nil {
			x = &RBNode{data: -1, color: BLACK, parent: y}
		}

		if y.parent != z {
			transplant(tree, y, x)
			y.right = z.right
			y.right.parent = y
		} else {
			x.parent = y
		}

		transplant(tree, z, y)
		y.left = z.left
		y.left.parent = y
		y.color = z.color
	}

	if yOriginalColor == BLACK {
		deleteFix(tree, x)
	}

	if x.data == -1 {
		if x.parent != nil {
			if x.parent.left == x {
				x.parent.left = nil
			} else if x.parent.right == x {
				x.parent.right = nil
			}
		}
	}
}

func printTree(node *RBNode, space int, gap int) {
	if node == nil {
		return
	}

	printTree(node.right, space+gap, gap)

	for i := 0; i < space; i++ {
		fmt.Print(" ")
	}

	fmt.Printf("%d(", node.data)
	if node.color == RED {
		fmt.Print("R")
	} else {
		fmt.Print("B")
	}
	fmt.Printf(")\n")

	printTree(node.left, space+gap, gap)
}

func printTreeStruct(tree *RBTree) {
	if tree.root == nil {
		fmt.Println("Дерево пустое")
		return
	}

	fmt.Println("Структура дерева:")
	printTree(tree.root, 0, 5)
}

// ==================== СТЕКИ ====================

func SCREATE(newStack *Stack, data int) {
	newStack.head = &SNode{data: data}
}

func SPUSH(list *Stack, data int) {
	newSNode := &SNode{data: data, next: list.head}
	list.head = newSNode
}

func SPOP(list *Stack) {
	if list.head == nil {
		return
	}
	address := list.head.next
	list.head = address
}

func SPRINT(list *Stack) {
	address := list.head
	for address != nil {
		fmt.Println(address.data)
		address = address.next
	}
}

// ==================== ОЧЕРЕДИ ====================

func QCREATE(newQueue *Queue, data int) {
	newQueue.head = &QNode{data: data}
	newQueue.tail = newQueue.head
}

func QPUSH(list *Queue, data int) {
	newQNode := &QNode{data: data, previous: list.tail}
	list.tail.next = newQNode
	list.tail = newQNode
}

func QPOP(list *Queue) {
	if list.head == nil {
		return
	}
	address := list.head.next
	list.head = address
}

func QPRINT(list *Queue) {
	address := list.head
	for {
		if address.next == nil {
			fmt.Println(address.data)
			return
		} else {
			fmt.Println(address.data)
			address = address.next
		}
	}
}

// ==================== ДВУСВЯЗНЫЕ СПИСКИ ====================

func LCREATE(newLList *LList, data int) {
	newLList.head = &LLNode{data: data}
	newLList.tail = newLList.head
}

func LGETINDEX(list *LList, index int) *LLNode {
	address := list.head
	for {
		if index == 0 {
			return address
		}
		if address.next == nil {
			if index != 0 {
				fmt.Println("Элемента с таким индексом нет")
				os.Exit(0)
			}
			return address
		} else {
			address = address.next
			index--
		}
	}
}

func LGETDATA(list *LList, data int) *LLNode {
	address := list.head
	for {
		if address.data == data {
			return address
		}
		if address.next == nil {
			if address.data != data {
				fmt.Println("Узел отсутствует в списке")
				os.Exit(0)
			}
			return address
		} else {
			address = address.next
		}
	}
}

func LADDTAIL(list *LList, data int) {
	newLLNode := &LLNode{data: data, previous: list.tail}
	list.tail.next = newLLNode
	list.tail = newLLNode
}

func LADDHEAD(list *LList, data int) {
	newLLNode := &LLNode{data: data, next: list.head}

	if list.head != nil {
		list.head.previous = newLLNode
	}

	list.head = newLLNode

	if list.tail == nil {
		list.tail = newLLNode
	}
}

func LADDAFTER(list *LList, ptr *LLNode, data int) {
	if ptr.next == nil {
		LADDTAIL(list, data)
		return
	}
	newLLNode := &LLNode{data: data, previous: ptr, next: ptr.next}
	ptr.next = newLLNode
	newLLNode.next.previous = newLLNode
}

func LADDBEFORE(list *LList, ptr *LLNode, data int) {
	if list.head == ptr {
		LADDHEAD(list, data)
		return
	}

	newLLNode := &LLNode{data: data, previous: ptr.previous, next: ptr}
	ptr.previous.next = newLLNode
	ptr.previous = newLLNode
}

func LDELTAIL(list *LList) {
	if list.tail == nil {
		return
	}
	address := list.tail.previous
	list.tail = address
	if address != nil {
		address.next = nil
	}
}

func LDELHEAD(list *LList) {
	if list.head == nil {
		return
	}
	address := list.head.next
	list.head = address
}

func LDELAFTER(list *LList, ptr *LLNode) {
	if ptr.next == nil {
		return
	}
	if ptr.next == list.tail {
		LDELTAIL(list)
		return
	}
	address := ptr.next.next
	ptr.next = address
	if address != nil {
		address.previous = ptr
	}
}

func LDELBEFORE(list *LList, ptr *LLNode) {
	if list.head == ptr {
		fmt.Println("Элемент первый в списке")
		return
	}
	if ptr.previous == list.head {
		LDELHEAD(list)
		return
	}
	address := ptr.previous.previous
	ptr.previous = address
	if address != nil {
		address.next = ptr
	}
}

func LDELDATA(list *LList, data int) {
	address := list.head
	if address.data == data {
		LDELHEAD(list)
		return
	}

	for {
		if address.next == nil {
			if address.data == data {
				LDELTAIL(list)
				return
			}
			fmt.Println("Элемента нет в списке")
			return
		}

		if address.data == data {
			address.previous.next = address.next
			address.next.previous = address.previous
			return
		}

		address = address.next
	}
}

func LPRINT(list *LList) {
	address := list.head
	for {
		if address.next == nil {
			fmt.Println(address.data)
			return
		} else {
			fmt.Println(address.data)
			address = address.next
		}
	}
}

// ==================== ОДНОСВЯЗНЫЕ СПИСКИ ====================

func FCREATE(newFList *FList, data int) {
	newFList.head = &FLNode{data: data}
}

func FGETINDEX(list *FList, index int) *FLNode {
	address := list.head
	for {
		if index == 0 {
			return address
		}
		if address.next == nil {
			if index != 0 {
				fmt.Println("Элемента с таким индексом нет")
				os.Exit(0)
			}
			return address
		} else {
			address = address.next
			index--
		}
	}
}

func FGETFLNodePARENT(list *FList, targetFLNode *FLNode) *FLNode {
	address := list.head
	for {
		if address.next == targetFLNode {
			return address
		}
		if address.next == nil {
			if address != targetFLNode {
				fmt.Println("Узел отсутствует в списке")
				os.Exit(0)
			}
			return address
		} else {
			address = address.next
		}
	}
}

func FGETDATA(list *FList, data int) *FLNode {
	address := list.head
	for {
		if address.data == data {
			return address
		}
		if address.next == nil {
			if address.data != data {
				fmt.Println("Узел отсутствует в списке")
				os.Exit(0)
			}
			return address
		} else {
			address = address.next
		}
	}
}

func FADDTAIL(list *FList, data int) {
	newFLNode := &FLNode{data: data}
	address := list.head
	for {
		if address.next == nil {
			address.next = newFLNode
			break
		} else {
			address = address.next
		}
	}
}

func FADDHEAD(list *FList, data int) {
	newFLNode := &FLNode{data: data, next: list.head}
	list.head = newFLNode
}

func FADDAFTER(ptr *FLNode, data int) {
	newFLNode := &FLNode{data: data, next: ptr.next}
	ptr.next = newFLNode
}

func FADDBEFORE(list *FList, ptr *FLNode, data int) {
	if list.head == ptr {
		FADDHEAD(list, data)
		return
	}
	newFLNode := &FLNode{data: data, next: ptr}
	FGETFLNodePARENT(list, ptr).next = newFLNode
}

func FDELTAIL(list *FList) {
	address := list.head
	for {
		if address.next.next == nil {
			address.next = nil
			break
		} else {
			address = address.next
		}
	}
}

func FDELHEAD(list *FList) {
	if list.head == nil {
		return
	}
	address := list.head.next
	list.head = address
}

func FDELAFTER(ptr *FLNode) {
	if ptr.next == nil {
		return
	}
	address := ptr.next.next
	ptr.next = address
}

func FDELBEFORE(list *FList, ptr *FLNode) {
	if list.head == ptr {
		fmt.Println("Элемент первый в списке")
		return
	}
	if list.head.next == ptr {
		FDELHEAD(list)
		return
	}
	//address := FGETFLNodePARENT(list, FGETFLNodePARENT(list, ptr))
	FGETFLNodePARENT(list, ptr).next = ptr
}

func FDELDATA(list *FList, data int) {
	address := list.head
	if address.data == data {
		FDELHEAD(list)
		return
	}

	for {
		if address.next == nil {
			fmt.Println("Элемента нет в списке")
			return
		}
		if address.next.data == data {
			FDELAFTER(address)
			return
		}
		address = address.next
	}
}

func FPRINT(list *FList) {
	address := list.head
	for {
		if address.next == nil {
			fmt.Println(address.data)
			return
		} else {
			fmt.Println(address.data)
			address = address.next
		}
	}
}

// ==================== МАССИВЫ ====================

func MCREATE() *Array {
	return &Array{data: make([]int, 0)}
}

func MSIZE(array *Array) {
	fmt.Printf("Size = %d\n", len(array.data))
}

func MPUSHEND(array *Array, a int) {
	array.data = append(array.data, a)
}

func MPUSH(array *Array, a int, index int) {
	currentSize := len(array.data)

	if index >= currentSize {
		for i := 0; i < index-currentSize; i++ {
			MPUSHEND(array, 0)
		}
		MPUSHEND(array, a)
	} else {
		array.data = append(array.data, 0)
		for i := currentSize; i > index; i-- {
			array.data[i] = array.data[i-1]
		}
		array.data[index] = a
	}
}

func MDEL(array *Array, index int) {
	currentSize := len(array.data)
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return
	} else {
		array.data = append(array.data[:index], array.data[index+1:]...)
	}
}

func MSWAP(array *Array, swapElement int, index int) {
	currentSize := len(array.data)
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return
	} else {
		array.data[index] = swapElement
	}
}

func MGET(array *Array, index int) int {
	currentSize := len(array.data)
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return 0
	} else {
		return array.data[index]
	}
}

func MPRINT(array *Array) {
	for i, value := range array.data {
		fmt.Printf("%d index = %d\n", value, i)
	}
}

// ==================== СИСТЕМА УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ ====================

func NewDatabase(filename string) *Database {
	return &Database{
		arrays:   make(map[string]*Array),
		flists:   make(map[string]*FList),
		llists:   make(map[string]*LList),
		queues:   make(map[string]*Queue),
		stacks:   make(map[string]*Stack),
		trees:    make(map[string]*RBTree),
		filename: filename,
	}
}

func (db *Database) saveToFile() {
	file, err := os.Create(db.filename)
	if err != nil {
		fmt.Printf("Ошибка, невозможно открыть файл %s\n", db.filename)
		return
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	defer writer.Flush()

	// Сохраняем массивы
	arrayCount := uint64(len(db.arrays))
	binary.Write(writer, binary.LittleEndian, arrayCount)
	for name, array := range db.arrays {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		size := uint64(len(array.data))
		binary.Write(writer, binary.LittleEndian, size)
		if size > 0 {
			for _, value := range array.data {
				binary.Write(writer, binary.LittleEndian, int32(value))
			}
		}
	}

	// Сохраняем односвязные списки
	flistCount := uint64(len(db.flists))
	binary.Write(writer, binary.LittleEndian, flistCount)
	for name, flist := range db.flists {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		count := 0
		current := flist.head
		for current != nil {
			count++
			current = current.next
		}

		binary.Write(writer, binary.LittleEndian, int32(count))
		current = flist.head
		for current != nil {
			binary.Write(writer, binary.LittleEndian, int32(current.data))
			current = current.next
		}
	}

	// Сохраняем двусвязные списки
	llistCount := uint64(len(db.llists))
	binary.Write(writer, binary.LittleEndian, llistCount)
	for name, llist := range db.llists {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		count := 0
		current := llist.head
		for current != nil {
			count++
			current = current.next
		}

		binary.Write(writer, binary.LittleEndian, int32(count))
		current = llist.head
		for current != nil {
			binary.Write(writer, binary.LittleEndian, int32(current.data))
			current = current.next
		}
	}

	// Сохраняем стеки
	stackCount := uint64(len(db.stacks))
	binary.Write(writer, binary.LittleEndian, stackCount)
	for name, stack := range db.stacks {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		count := 0
		current := stack.head
		for current != nil {
			count++
			current = current.next
		}

		binary.Write(writer, binary.LittleEndian, int32(count))
		temp := make([]int, count)
		current = stack.head
		for i := count - 1; i >= 0; i-- {
			temp[i] = current.data
			current = current.next
		}
		for _, value := range temp {
			binary.Write(writer, binary.LittleEndian, int32(value))
		}
	}

	// Сохраняем очереди
	queueCount := uint64(len(db.queues))
	binary.Write(writer, binary.LittleEndian, queueCount)
	for name, queue := range db.queues {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		count := 0
		current := queue.head
		for current != nil {
			count++
			current = current.next
		}

		binary.Write(writer, binary.LittleEndian, int32(count))
		current = queue.head
		for current != nil {
			binary.Write(writer, binary.LittleEndian, int32(current.data))
			current = current.next
		}
	}

	// Сохраняем деревья
	treeCount := uint64(len(db.trees))
	binary.Write(writer, binary.LittleEndian, treeCount)
	for name, tree := range db.trees {
		nameBytes := []byte(name)
		nameLength := uint64(len(nameBytes))
		binary.Write(writer, binary.LittleEndian, nameLength)
		writer.Write(nameBytes)

		db.saveTreeToFile(writer, tree.root)
	}

	fmt.Printf("База данных сохранена %s\n", db.filename)
}

func (db *Database) saveTreeToFile(writer *bufio.Writer, node *RBNode) {
	if node == nil {
		marker := int32(-1)
		binary.Write(writer, binary.LittleEndian, marker)
		return
	}

	binary.Write(writer, binary.LittleEndian, int32(node.data))
	color := int32(0)
	if node.color == BLACK {
		color = 1
	}
	binary.Write(writer, binary.LittleEndian, color)

	db.saveTreeToFile(writer, node.left)
	db.saveTreeToFile(writer, node.right)
}

func (db *Database) loadFromFile() {
	file, err := os.Open(db.filename)
	if err != nil {
		fmt.Printf("Файл %s ненайден, создан новый файл\n", db.filename)
		return
	}
	defer file.Close()

	reader := bufio.NewReader(file)

	// Загружаем массивы
	var arrayCount uint64
	binary.Read(reader, binary.LittleEndian, &arrayCount)
	for i := uint64(0); i < arrayCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		var size uint64
		binary.Read(reader, binary.LittleEndian, &size)

		arr := MCREATE()
		if size > 0 {
			arr.data = make([]int, size)
			for j := uint64(0); j < size; j++ {
				var value int32
				binary.Read(reader, binary.LittleEndian, &value)
				arr.data[j] = int(value)
			}
		}
		db.arrays[name] = arr
	}

	// Загружаем односвязные списки
	var flistCount uint64
	binary.Read(reader, binary.LittleEndian, &flistCount)
	for i := uint64(0); i < flistCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		var count int32
		binary.Read(reader, binary.LittleEndian, &count)

		flist := &FList{}
		if count > 0 {
			var data int32
			binary.Read(reader, binary.LittleEndian, &data)
			FCREATE(flist, int(data))

			for j := int32(1); j < count; j++ {
				binary.Read(reader, binary.LittleEndian, &data)
				FADDTAIL(flist, int(data))
			}
		}
		db.flists[name] = flist
	}

	// Загружаем двусвязные списки
	var llistCount uint64
	binary.Read(reader, binary.LittleEndian, &llistCount)
	for i := uint64(0); i < llistCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		var count int32
		binary.Read(reader, binary.LittleEndian, &count)

		llist := &LList{}
		if count > 0 {
			var data int32
			binary.Read(reader, binary.LittleEndian, &data)
			LCREATE(llist, int(data))

			for j := int32(1); j < count; j++ {
				binary.Read(reader, binary.LittleEndian, &data)
				LADDTAIL(llist, int(data))
			}
		}
		db.llists[name] = llist
	}

	// Загружаем стеки
	var stackCount uint64
	binary.Read(reader, binary.LittleEndian, &stackCount)
	for i := uint64(0); i < stackCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		var count int32
		binary.Read(reader, binary.LittleEndian, &count)

		stack := &Stack{}
		if count > 0 {
			temp := make([]int, count)
			for j := int32(0); j < count; j++ {
				var value int32
				binary.Read(reader, binary.LittleEndian, &value)
				temp[j] = int(value)
			}
			SCREATE(stack, temp[0])
			for j := int32(1); j < count; j++ {
				SPUSH(stack, temp[j])
			}
		}
		db.stacks[name] = stack
	}

	// Загружаем очереди
	var queueCount uint64
	binary.Read(reader, binary.LittleEndian, &queueCount)
	for i := uint64(0); i < queueCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		var count int32
		binary.Read(reader, binary.LittleEndian, &count)

		queue := &Queue{}
		if count > 0 {
			var data int32
			binary.Read(reader, binary.LittleEndian, &data)
			QCREATE(queue, int(data))

			for j := int32(1); j < count; j++ {
				binary.Read(reader, binary.LittleEndian, &data)
				QPUSH(queue, int(data))
			}
		}
		db.queues[name] = queue
	}

	// Загружаем деревья
	var treeCount uint64
	binary.Read(reader, binary.LittleEndian, &treeCount)
	for i := uint64(0); i < treeCount; i++ {
		var nameLength uint64
		binary.Read(reader, binary.LittleEndian, &nameLength)
		nameBytes := make([]byte, nameLength)
		reader.Read(nameBytes)
		name := string(nameBytes)

		tree := &RBTree{}
		tree.root = db.loadTreeFromFile(reader)
		db.trees[name] = tree
	}

	fmt.Printf("База данных загружена из %s\n", db.filename)
}

func (db *Database) loadTreeFromFile(reader *bufio.Reader) *RBNode {
	var data int32
	err := binary.Read(reader, binary.LittleEndian, &data)
	if err == io.EOF {
		return nil
	}

	if data == -1 {
		return nil
	}

	var color int32
	binary.Read(reader, binary.LittleEndian, &color)

	node := &RBNode{
		data:  int(data),
		color: RED,
	}
	if color == 1 {
		node.color = BLACK
	}

	node.left = db.loadTreeFromFile(reader)
	if node.left != nil {
		node.left.parent = node
	}

	node.right = db.loadTreeFromFile(reader)
	if node.right != nil {
		node.right.parent = node
	}

	return node
}

func (db *Database) executeQuery(query string) string {
	tokens := strings.Fields(query)
	if len(tokens) == 0 {
		return "Ошибка, запрос пуст"
	}

	command := strings.ToUpper(tokens[0])

	// ==================== МАССИВЫ ====================
	if command == "MCREATE" && len(tokens) == 2 {
		name := tokens[1]
		db.arrays[name] = MCREATE()
		return "OK"
	} else if command == "MSIZE" && len(tokens) == 2 {
		name := tokens[1]
		if array, exists := db.arrays[name]; exists {
			return fmt.Sprintf("%d", len(array.data))
		}
		return "Массив ненайден"
	} else if command == "MPUSHEND" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if _, exists := db.arrays[name]; !exists {
			db.arrays[name] = MCREATE()
		}
		MPUSHEND(db.arrays[name], value)
		return fmt.Sprintf("%d", value)
	} else if command == "MPUSH" && len(tokens) == 4 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if _, exists := db.arrays[name]; !exists {
			db.arrays[name] = MCREATE()
		}
		MPUSH(db.arrays[name], value, index)
		return fmt.Sprintf("%d", value)
	} else if command == "MDEL" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])

		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		MDEL(db.arrays[name], index)
		return "OK"
	} else if command == "MSWAP" && len(tokens) == 4 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		index, _ := strconv.Atoi(tokens[3])

		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		MSWAP(db.arrays[name], value, index)
		return "OK"
	} else if command == "MGET" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])

		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		value := MGET(db.arrays[name], index)
		return fmt.Sprintf("%d", value)
	} else if command == "MPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if array, exists := db.arrays[name]; exists {
			MPRINT(array)
			return "OK"
		}
		return "Массив не найден"
	}

	// ==================== ОДНОСВЯЗНЫЕ СПИСКИ ====================
	if command == "FCREATE" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		flist := &FList{}
		FCREATE(flist, value)
		db.flists[name] = flist
		return fmt.Sprintf("%d", value)
	} else if command == "FGETINDEX" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			node := FGETINDEX(flist, index)
			return fmt.Sprintf("%d", node.data)
		}
		return "Список не найден"
	} else if command == "FGETDATA" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			node := FGETDATA(flist, value)
			return fmt.Sprintf("%d", node.data)
		}
		return "Список не найден"
	} else if command == "FADDTAIL" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			FADDTAIL(flist, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FADDHEAD" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			FADDHEAD(flist, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FADDAFTERINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			FADDAFTER(targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FADDAFTERDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			FADDAFTER(targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FADDBEFOREINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			FADDBEFORE(flist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FADDBEFOREDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			FADDBEFORE(flist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "FDELTAIL" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FDELTAIL(flist)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELHEAD" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FDELHEAD(flist)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELAFTERINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			FDELAFTER(targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELAFTERDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			FDELAFTER(targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELBEFOREINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			FDELBEFORE(flist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELBEFOREDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])

		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			FDELBEFORE(flist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FDELDATA" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		if flist, exists := db.flists[name]; exists {
			FDELDATA(flist, value)
			return "OK"
		}
		return "Список не найден"
	} else if command == "FPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FPRINT(flist)
			return "OK"
		}
		return "Список не найден"
	}

	// ==================== ДВУСВЯЗНЫЕ СПИСКИ ====================
	if command == "LCREATE" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		llist := &LList{}
		LCREATE(llist, value)
		db.llists[name] = llist
		return fmt.Sprintf("%d", value)
	} else if command == "LGETINDEX" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			node := LGETINDEX(llist, index)
			return fmt.Sprintf("%d", node.data)
		}
		return "Список не найден"
	} else if command == "LGETDATA" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			node := LGETDATA(llist, value)
			return fmt.Sprintf("%d", node.data)
		}
		return "Список не найден"
	} else if command == "LADDTAIL" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			LADDTAIL(llist, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LADDHEAD" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			LADDHEAD(llist, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LADDAFTERINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			LADDAFTER(llist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LADDAFTERDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			LADDAFTER(llist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LADDBEFOREINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			LADDBEFORE(llist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LADDBEFOREDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])
		value, _ := strconv.Atoi(tokens[3])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			LADDBEFORE(llist, targetNode, value)
			return fmt.Sprintf("%d", value)
		}
		return "Список не найден"
	} else if command == "LDELTAIL" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LDELTAIL(llist)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELHEAD" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LDELHEAD(llist)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELAFTERINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			LDELAFTER(llist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELAFTERDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			LDELAFTER(llist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELBEFOREINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			LDELBEFORE(llist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELBEFOREDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue, _ := strconv.Atoi(tokens[2])

		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			LDELBEFORE(llist, targetNode)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LDELDATA" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		if llist, exists := db.llists[name]; exists {
			LDELDATA(llist, value)
			return "OK"
		}
		return "Список не найден"
	} else if command == "LPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LPRINT(llist)
			return "OK"
		}
		return "Список не найден"
	}

	// ==================== ОЧЕРЕДИ ====================
	if command == "QCREATE" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])
		queue := &Queue{}
		QCREATE(queue, value)
		db.queues[name] = queue
		return fmt.Sprintf("%d", value)
	} else if command == "QPUSH" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if queue, exists := db.queues[name]; exists {
			QPUSH(queue, value)
			return fmt.Sprintf("%d", value)
		}
		return "Очередь не найдена"
	} else if command == "QPOP" && len(tokens) == 2 {
		name := tokens[1]
		if queue, exists := db.queues[name]; exists && queue.head != nil {
			value := queue.head.data
			QPOP(queue)
			return fmt.Sprintf("%d", value)
		}
		return "Очередь не существует или пуста"
	} else if command == "QPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if queue, exists := db.queues[name]; exists {
			QPRINT(queue)
			return "OK"
		}
		return "Очередь не найдена"
	}

	// ==================== СТЕКИ ====================
	if command == "SCREATE" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if _, exists := db.stacks[name]; exists {
			return "Стэк с таким именем уже существует"
		}
		stack := &Stack{}
		SCREATE(stack, value)
		db.stacks[name] = stack
		return fmt.Sprintf("%d", value)
	} else if command == "SPUSH" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if stack, exists := db.stacks[name]; exists {
			SPUSH(stack, value)
			return fmt.Sprintf("%d", value)
		}
		return "Стэк не найден"
	} else if command == "SPOP" && len(tokens) == 2 {
		name := tokens[1]
		if stack, exists := db.stacks[name]; exists && stack.head != nil {
			value := stack.head.data
			SPOP(stack)
			return fmt.Sprintf("%d", value)
		}
		return "Стэк пуст или не существует"
	} else if command == "SPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if stack, exists := db.stacks[name]; exists {
			SPRINT(stack)
			return "OK"
		}
		return "Стэк не найден"
	}

	// ==================== ДЕРЕВЬЯ ====================
	if command == "TADD" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if _, exists := db.trees[name]; !exists {
			db.trees[name] = &RBTree{}
		}
		addRBNode(db.trees[name], value)
		return fmt.Sprintf("%d", value)
	} else if command == "TDEL" && len(tokens) == 3 {
		name := tokens[1]
		value, _ := strconv.Atoi(tokens[2])

		if _, exists := db.trees[name]; !exists {
			return "Дерево не найдено"
		}
		delRBNode(db.trees[name], value)
		return fmt.Sprintf("%d", value)
	} else if command == "TPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if tree, exists := db.trees[name]; exists {
			printTreeStruct(tree)
			return "OK"
		}
		return "Дерево не найдено"
	}

	return "Ошибка, неизвестная комманда или неправильные аргументы"
}

// ==================== ОБРАБОТКА АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ ====================

func printUsage() {
	fmt.Println("Использование: ./lab1 --file <имя_файла> --query '<команда>'")
	fmt.Println()
	fmt.Println("МАССИВЫ:")
	fmt.Println("  MCREATE <имя>                          - Создать массив")
	fmt.Println("  MSIZE <имя>                            - Получить размер массива")
	fmt.Println("  MPUSHEND <имя> <значение>              - Добавить в конец массива")
	fmt.Println("  MPUSH <имя> <индекс> <значение>        - Добавить по индексу")
	fmt.Println("  MDEL <имя> <индекс>                    - Удалить по индексу")
	fmt.Println("  MSWAP <имя> <значение> <индекс>        - Заменить элемент по индексу")
	fmt.Println("  MGET <имя> <индекс>                    - Получить элемент по индексу")
	fmt.Println("  MPRINT <имя>                           - Вывести массив")
	fmt.Println()

	fmt.Println("ОДНОСВЯЗНЫЕ СПИСКИ:")
	fmt.Println("  FCREATE <имя> <значение>               - Создать список")
	fmt.Println("  FGETINDEX <имя> <индекс>               - Получить значение по индексу")
	fmt.Println("  FGETDATA <имя> <значение>              - Найти узел по значению")
	fmt.Println("  FADDTAIL <имя> <значение>              - Добавить в конец")
	fmt.Println("  FADDHEAD <имя> <значение>              - Добавить в начало")
	fmt.Println("  FADDAFTERINDEX <имя> <индекс> <знач>   - Добавить после индекса")
	fmt.Println("  FADDAFTERDATA <имя> <знач> <нов_знач>  - Добавить после значения")
	fmt.Println("  FADDBEFOREINDEX <имя> <индекс> <знач>  - Добавить до индекса")
	fmt.Println("  FADDBEFOREDATA <имя> <знач> <нов_знач> - Добавить до значения")
	fmt.Println("  FDELTAIL <имя>                         - Удалить хвост")
	fmt.Println("  FDELHEAD <имя>                         - Удалить голову")
	fmt.Println("  FDELAFTERINDEX <имя> <индекс>          - Удалить после индекса")
	fmt.Println("  FDELAFTERDATA <имя> <значение>         - Удалить после значения")
	fmt.Println("  FDELBEFOREINDEX <имя> <индекс>         - Удалить до индекса")
	fmt.Println("  FDELBEFOREDATA <имя> <значение>        - Удалить до значения")
	fmt.Println("  FDELDATA <имя> <значение>              - Удалить по значению")
	fmt.Println("  FPRINT <имя>                           - Вывести список")
	fmt.Println()

	fmt.Println("ДВУСВЯЗНЫЕ СПИСКИ:")
	fmt.Println("  LCREATE <имя> <значение>               - Создать список")
	fmt.Println("  LGETINDEX <имя> <индекс>               - Получить значение по индексу")
	fmt.Println("  LGETDATA <имя> <значение>              - Найти узел по значению")
	fmt.Println("  LADDTAIL <имя> <значение>              - Добавить в конец")
	fmt.Println("  LADDHEAD <имя> <значение>              - Добавить в начало")
	fmt.Println("  LADDAFTERINDEX <имя> <индекс> <знач>   - Добавить после индекса")
	fmt.Println("  LADDAFTERDATA <имя> <знач> <нов_знач>  - Добавить после значения")
	fmt.Println("  LADDBEFOREINDEX <имя> <индекс> <знач>  - Добавить до индекса")
	fmt.Println("  LADDBEFOREDATA <имя> <знач> <нов_знач> - Добавить до значения")
	fmt.Println("  LDELTAIL <имя>                         - Удалить хвост")
	fmt.Println("  LDELHEAD <имя>                         - Удалить голову")
	fmt.Println("  LDELAFTERINDEX <имя> <индекс>          - Удалить после индекса")
	fmt.Println("  LDELAFTERDATA <имя> <значение>         - Удалить после значения")
	fmt.Println("  LDELBEFOREINDEX <имя> <индекс>         - Удалить до индекса")
	fmt.Println("  LDELBEFOREDATA <имя> <значение>        - Удалить до значения")
	fmt.Println("  LDELDATA <имя> <значение>              - Удалить по значению")
	fmt.Println("  LPRINT <имя>                           - Вывести список")
	fmt.Println()

	fmt.Println("ОЧЕРЕДИ:")
	fmt.Println("  QCREATE <имя> <значение>               - Создать очередь")
	fmt.Println("  QPUSH <имя> <значение>                 - Добавить в очередь")
	fmt.Println("  QPOP <имя>                             - Извлечь из очереди")
	fmt.Println("  QPRINT <имя>                           - Вывести очередь")
	fmt.Println()

	fmt.Println("СТЕКИ:")
	fmt.Println("  SCREATE <имя> <значение>               - Создать стек")
	fmt.Println("  SPUSH <имя> <значение>                 - Добавить в стек")
	fmt.Println("  SPOP <имя>                             - Извлечь из стека")
	fmt.Println("  SPRINT <имя>                           - Вывести стек")
	fmt.Println()

	fmt.Println("КРАСНО-ЧЕРНЫЕ ДЕРЕВЬЯ:")
	fmt.Println("  TADD <имя> <значение>                  - Добавить в дерево")
	fmt.Println("  TDEL <имя> <значение>                  - Удалить из дерева")
	fmt.Println("  TPRINT <имя>                           - Вывести дерево")
	fmt.Println()
}

func main() {
	var filename, query string

	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		if arg == "--file" && i+1 < len(os.Args) {
			filename = os.Args[i+1]
			i++
		} else if arg == "--query" && i+1 < len(os.Args) {
			query = os.Args[i+1]
			i++
		} else if arg == "--help" {
			printUsage()
			return
		}
	}

	if filename == "" || query == "" {
		printUsage()
		os.Exit(1)
	}

	db := NewDatabase(filename)
	db.loadFromFile()

	result := db.executeQuery(query)
	fmt.Println(result)

	db.saveToFile()
}
