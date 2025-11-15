package main

import (
	"fmt"
)

type QNode struct {
	data     string
	previous *QNode
	next     *QNode
}

// Конструктор
func NewQNode(val string, prev *QNode, nxt *QNode) *QNode {
	return &QNode{
		data:     val,
		previous: prev,
		next:     nxt,
	}
}

type Queue struct {
	tail *QNode
	head *QNode
}

// Конструктор
func NewQueue() *Queue {
	return &Queue{
		tail: nil,
		head: nil,
	}
}

// Конструктор копирования
func (q *Queue) Copy() *Queue {
	newQueue := &Queue{
		tail: nil,
		head: nil,
	}

	if q.head != nil {
		currentOther := q.head
		var last *QNode

		// Копируем первый элемент
		newQueue.head = NewQNode(currentOther.data, nil, nil)
		last = newQueue.head
		currentOther = currentOther.next

		// Копируем остальные элементы
		for currentOther != nil {
			newNode := NewQNode(currentOther.data, last, nil)
			last.next = newNode
			last = newNode
			currentOther = currentOther.next
		}
		newQueue.tail = last
	}

	return newQueue
}

// Оператор присваивания
func (q *Queue) Assign(other *Queue) *Queue {
	if q != other {
		// Очищаем текущий список
		current := q.head
		for current != nil {
			next := current.next
			current = nil
			current = next
		}
		q.head = nil
		q.tail = nil

		// Копируем элементы из другого списка
		if other.head != nil {
			currentOther := other.head
			var last *QNode

			q.head = NewQNode(currentOther.data, nil, nil)
			last = q.head
			currentOther = currentOther.next

			for currentOther != nil {
				newNode := NewQNode(currentOther.data, last, nil)
				last.next = newNode
				last = newNode
				currentOther = currentOther.next
			}
			q.tail = last
		}
	}
	return q
}

func QCREATE(newQueue *Queue, data string) {
	newQueue.head = NewQNode(data, nil, nil)
	newQueue.tail = newQueue.head
}

func QPUSH(queue *Queue, data string) {
	newQNode := NewQNode(data, queue.tail, nil)
	queue.tail.next = newQNode
	queue.tail = newQNode
}

func QPOP(queue *Queue) {
	if queue.head == nil {
		fmt.Println("Очередь пуста")
		return
	}

	Address := queue.head.next
	queue.head = Address
	if Address != nil {
		Address.previous = nil
	} else {
		// Если удалили последний элемент
		queue.tail = nil
	}
}

func QPRINT(queue *Queue) {
	if queue.head == nil {
		fmt.Println("Очередь пуста")
		return
	}

	Address := queue.head
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

// Дополнительные методы для удобства работы с очередью
func QFRONT(queue *Queue) string {
	if queue.head == nil {
		fmt.Println("Очередь пуста")
		return ""
	}
	return queue.head.data
}

func QBACK(queue *Queue) string {
	if queue.tail == nil {
		fmt.Println("Очередь пуста")
		return ""
	}
	return queue.tail.data
}

func QEMPTY(queue *Queue) bool {
	return queue.head == nil
}
