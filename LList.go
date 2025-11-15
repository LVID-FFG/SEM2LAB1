package main

import (
	"fmt"
)

type LLNode struct {
	data     string
	previous *LLNode
	next     *LLNode
}

// Конструктор
func NewLLNode(val string, prev *LLNode, nxt *LLNode) *LLNode {
	return &LLNode{
		data:     val,
		previous: prev,
		next:     nxt,
	}
}

type LList struct {
	tail *LLNode
	head *LLNode
}

// Конструктор
func NewLList() *LList {
	return &LList{
		tail: nil,
		head: nil,
	}
}

// Конструктор копирования
func (l *LList) Copy() *LList {
	newList := &LList{
		tail: nil,
		head: nil,
	}

	if l.head != nil {
		currentOther := l.head
		var last *LLNode

		// Копируем первый элемент
		newList.head = NewLLNode(currentOther.data, nil, nil)
		last = newList.head
		currentOther = currentOther.next

		// Копируем остальные элементы
		for currentOther != nil {
			newNode := NewLLNode(currentOther.data, last, nil)
			last.next = newNode
			last = newNode
			currentOther = currentOther.next
		}
		newList.tail = last
	}

	return newList
}

// Оператор присваивания
func (l *LList) Assign(other *LList) *LList {
	if l != other {
		// Очищаем текущий список
		current := l.head
		for current != nil {
			next := current.next
			current = nil
			current = next
		}
		l.head = nil
		l.tail = nil

		// Копируем элементы из другого списка
		if other.head != nil {
			currentOther := other.head
			var last *LLNode

			l.head = NewLLNode(currentOther.data, nil, nil)
			last = l.head
			currentOther = currentOther.next

			for currentOther != nil {
				newNode := NewLLNode(currentOther.data, last, nil)
				last.next = newNode
				last = newNode
				currentOther = currentOther.next
			}
			l.tail = last
		}
	}
	return l
}

func LCREATE(newLList *LList, data string) {
	newLList.head = NewLLNode(data, nil, nil)
	newLList.tail = newLList.head
}

func LGETINDEX(list *LList, index int) *LLNode {
	Address := list.head
	for {
		if index == 0 {
			return Address
		}
		if Address.next == nil {
			if index != 0 {
				fmt.Println("Элемента с таким индексом нет")
				return nil
			}
			return Address
		} else {
			Address = Address.next
			index--
		}
	}
}

func LGETDATA(list *LList, data string) *LLNode {
	Address := list.head
	for {
		if Address.data == data {
			return Address
		}
		if Address.next == nil {
			if Address.data != data {
				fmt.Println("Узел отсутствует в списке")
				return nil
			}
			return Address
		} else {
			Address = Address.next
		}
	}
}

func LADDTAIL(list *LList, data string) {
	newLLNode := NewLLNode(data, list.tail, nil)
	list.tail.next = newLLNode
	list.tail = newLLNode
}

func LADDHEAD(list *LList, data string) {
	newLLNode := NewLLNode(data, nil, list.head)

	// Обновляем previous у старой головы
	if list.head != nil {
		list.head.previous = newLLNode
	}

	list.head = newLLNode

	// Если список был пуст (tail = nil), обновляем tail
	if list.tail == nil {
		list.tail = newLLNode
	}
}

func LADDAFTER(list *LList, ptr *LLNode, data string) {
	if ptr.next == nil {
		LADDTAIL(list, data)
		return
	}
	newLLNode := NewLLNode(data, ptr, ptr.next)
	ptr.next = newLLNode
	newLLNode.next.previous = newLLNode
}

func LADDBEFORE(list *LList, ptr *LLNode, data string) {
	if list.head == ptr {
		LADDHEAD(list, data)
		return
	}

	newLLNode := NewLLNode(data, ptr.previous, ptr)

	// Обновляем указатель next у предыдущего узла
	ptr.previous.next = newLLNode

	// Обновляем указатель previous у текущего узла
	ptr.previous = newLLNode
}

func LDELTAIL(list *LList) {
	if list.tail == nil {
		return
	}
	Address := list.tail.previous
	list.tail = Address
	if Address != nil {
		Address.next = nil
	} else {
		// Если удалили последний элемент
		list.head = nil
	}
}

func LDELHEAD(list *LList) {
	if list.head == nil {
		return
	}
	Address := list.head.next
	list.head = Address
	if Address != nil {
		Address.previous = nil
	} else {
		// Если удалили последний элемент
		list.tail = nil
	}
}

func LDELAFTER(list *LList, ptr *LLNode) {
	if ptr.next == nil {
		return
	}
	if ptr.next == list.tail {
		LDELTAIL(list)
		return
	}
	Address := ptr.next.next
	ptr.next = Address
	if Address != nil {
		Address.previous = ptr
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
	Address := ptr.previous.previous
	ptr.previous = Address
	if Address != nil {
		Address.next = ptr
	}
}

func LDELDATA(list *LList, data string) {
	if list.head == nil {
		fmt.Println("Список пуст")
		return
	}

	Address := list.head
	if Address.data == data {
		LDELHEAD(list)
		return
	}

	for {
		if Address.next == nil {
			if Address.data == data {
				LDELTAIL(list)
				return
			}
			fmt.Println("Элемента нет в списке")
			return
		}

		if Address.data == data {
			Address.previous.next = Address.next
			Address.next.previous = Address.previous
			return
		}

		Address = Address.next
	}
}

func LPRINT(list *LList) {
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
