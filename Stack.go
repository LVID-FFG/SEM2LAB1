package main

import (
	"fmt"
)

type SNode struct {
	data string
	next *SNode
}

// Конструктор
func NewSNode(val string, nextPtr *SNode) *SNode {
	return &SNode{
		data: val,
		next: nextPtr,
	}
}

type Stack struct {
	head *SNode
}

// Конструктор
func NewStack() *Stack {
	return &Stack{
		head: nil,
	}
}

// Конструктор копирования
func (s *Stack) Copy() *Stack {
	newStack := &Stack{
		head: nil,
	}

	current := s.head
	var last *SNode

	for current != nil {
		newNode := NewSNode(current.data, nil)
		if newStack.head == nil {
			newStack.head = newNode
		} else {
			last.next = newNode
		}
		last = newNode
		current = current.next
	}

	return newStack
}

// Оператор присваивания
func (s *Stack) Assign(other *Stack) *Stack {
	if s != other {
		// Копируем элементы
		current := other.head
		var last *SNode

		for current != nil {
			newNode := NewSNode(current.data, nil)
			if s.head == nil {
				s.head = newNode
			} else {
				last.next = newNode
			}
			last = newNode
			current = current.next
		}
	}
	return s
}

func SCREATE(newStack *Stack, data string) {
	newStack.head = NewSNode(data, nil)
}

func SPUSH(stack *Stack, data string) {
	newSNode := NewSNode(data, stack.head)
	stack.head = newSNode
}

func SPOP(stack *Stack) {
	if stack.head == nil {
		fmt.Println("Стек пуст")
		return
	}

	Address := stack.head.next
	stack.head = Address
}

func SPRINT(stack *Stack) {
	Address := stack.head
	for Address != nil {
		fmt.Println(Address.data)
		Address = Address.next
	}
}

// Дополнительные методы для удобства работы со стеком
func STOP(stack *Stack) string {
	if stack.head == nil {
		fmt.Println("Стек пуст")
		return ""
	}
	return stack.head.data
}

func SEMPTY(stack *Stack) bool {
	return stack.head == nil
}
