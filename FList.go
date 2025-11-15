package main

import (
	"fmt"
)

type FLNode struct {
	data string
	next *FLNode
}

// Конструктор
func NewFLNode(val string, nextPtr *FLNode) *FLNode {
	return &FLNode{
		data: val,
		next: nextPtr,
	}
}

type FList struct {
	head *FLNode
}

// Конструктор
func NewFList() *FList {
	return &FList{
		head: nil,
	}
}

// Конструктор копирования
func (f *FList) Copy() *FList {
	newList := &FList{
		head: nil,
	}

	current := f.head
	var last *FLNode

	for current != nil {
		newNode := NewFLNode(current.data, nil)
		if newList.head == nil {
			newList.head = newNode
		} else {
			last.next = newNode
		}
		last = newNode
		current = current.next
	}

	return newList
}

// Аналог оператора присваивания
func (f *FList) Assign(other *FList) *FList {
	if f != other {
		// Копируем элементы
		current := other.head
		var last *FLNode

		for current != nil {
			newNode := NewFLNode(current.data, nil)
			if f.head == nil {
				f.head = newNode
			} else {
				last.next = newNode
			}
			last = newNode
			current = current.next
		}
	}
	return f
}

func FCREATE(newFList *FList, data string) {
	newFList.head = NewFLNode(data, nil)
}

func FGETINDEX(list *FList, index int) *FLNode {
	Address := list.head
	currentIndex := 0

	for Address != nil {
		if currentIndex == index {
			return Address
		}
		Address = Address.next
		currentIndex++
	}

	fmt.Println("Элемента с таким индексом нет")
	return nil
}

func FGETFLNodePARENT(list *FList, targetFLNode *FLNode) *FLNode {
	if list.head == nil || list.head == targetFLNode {
		return list.head
	}

	Address := list.head
	for Address.next != nil {
		if Address.next == targetFLNode {
			return Address
		}
		Address = Address.next
	}

	fmt.Println("Узел отсутствует в списке")
	return nil
}

func FGETDATA(list *FList, data string) *FLNode {
	Address := list.head
	for Address != nil {
		if Address.data == data {
			return Address
		}
		Address = Address.next
	}

	fmt.Println("Узел отсутствует в списке")
	return nil
}

func FADDTAIL(list *FList, data string) {
	newFLNode := NewFLNode(data, nil)

	if list.head == nil {
		list.head = newFLNode
		return
	}

	Address := list.head
	for Address.next != nil {
		Address = Address.next
	}
	Address.next = newFLNode
}

func FADDHEAD(list *FList, data string) {
	newFLNode := NewFLNode(data, list.head)
	list.head = newFLNode
}

func FADDAFTER(ptr *FLNode, data string) {
	newFLNode := NewFLNode(data, ptr.next)
	ptr.next = newFLNode
}

func FADDBEFORE(list *FList, ptr *FLNode, data string) {
	if list.head == ptr {
		FADDHEAD(list, data)
		return
	}

	parent := FGETFLNodePARENT(list, ptr)
	if parent != nil {
		newFLNode := NewFLNode(data, ptr)
		parent.next = newFLNode
	}
}

func FDELTAIL(list *FList) {
	if list.head == nil {
		return
	}

	if list.head.next == nil {
		list.head = nil
		return
	}

	Address := list.head
	for Address.next.next != nil {
		Address = Address.next
	}

	Address.next = nil
}

func FDELHEAD(list *FList) {
	if list.head == nil {
		return
	}

	Address := list.head.next
	list.head = Address
}

func FDELAFTER(ptr *FLNode) {
	if ptr.next == nil {
		return
	}

	Address := ptr.next.next
	ptr.next = Address
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

	parent := FGETFLNodePARENT(list, ptr)
	if parent != nil {
		grandParent := FGETFLNodePARENT(list, parent)
		if grandParent != nil {
			grandParent.next = ptr
		}
	}
}

func FDELDATA(list *FList, data string) {
	if list.head == nil {
		fmt.Println("Список пуст")
		return
	}

	if list.head.data == data {
		FDELHEAD(list)
		return
	}

	Address := list.head
	for Address.next != nil {
		if Address.next.data == data {
			FDELAFTER(Address)
			return
		}
		Address = Address.next
	}

	fmt.Println("Элемента нет в списке")
}

func isListValid(list *FList) bool {
	if list.head == nil {
		return true
	}

	slow := list.head
	fast := list.head

	for fast != nil && fast.next != nil {
		slow = slow.next
		fast = fast.next.next

		if slow == fast {
			fmt.Println("DEBUG: Cycle detected in list!")
			return false
		}
	}
	return true
}

func FPRINT(list *FList) {
	Address := list.head
	for {
		if Address.next == nil {
			fmt.Println(Address.data)
			return
		} else {
			fmt.Println(Address.data)
			Address = Address.next
		}
	}
}
