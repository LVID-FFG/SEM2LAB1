package main

import (
	"fmt"
)

type Array struct {
	data []string
	size int
}

// Конструктор по умолчанию - создает пустой массив
func NewArray() *Array {
	return &Array{
		data: nil,
		size: 0,
	}
}

// Конструктор копирования
func (a *Array) Copy() *Array {
	newArray := &Array{
		size: a.size,
	}

	if a.data == nil {
		newArray.data = nil
	} else {
		newArray.data = make([]string, a.size)
		copy(newArray.data, a.data)
	}

	return newArray
}

// Аналог деструктора - очистка ресурсов
func (a *Array) Clear() {
	a.data = nil
	a.size = 0
}

func MSIZE(array *Array) {
	fmt.Printf("Size = %d\n", array.size)
}

func MPUSHEND(array *Array, str string) {
	if array.data == nil {
		// Если массив пустой, создаем с одним элементом
		array.data = []string{str}
		array.size = 1
		return
	}

	// В Go слайсы автоматически управляют памятью
	array.data = append(array.data, str)
	array.size = len(array.data)
}

func MPUSH(array *Array, str string, index int) {
	if array.data == nil {
		// Если массив пустой, создаем и добавляем элемент
		array.data = []string{str}
		array.size = 1
		return
	}

	currentSize := array.size

	if index >= currentSize {
		// Если индекс за пределами - добавляем в конец
		MPUSHEND(array, str)
	} else {
		// Вставка в середину массива
		newData := make([]string, currentSize+1)

		// Копируем элементы до индекса
		copy(newData[:index], array.data[:index])

		// Вставляем новый элемент
		newData[index] = str

		// Копируем оставшиеся элементы
		copy(newData[index+1:], array.data[index:])

		array.data = newData
		array.size = currentSize + 1
	}
}

func MDEL(array *Array, index int) {
	if array.data == nil {
		fmt.Println("Массив пуст")
		return
	}

	currentSize := array.size
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return
	}

	if currentSize == 1 {
		// Если удаляем последний элемент
		array.data = nil
		array.size = 0
		return
	}

	// Создаем новый массив на один элемент меньше
	newData := make([]string, currentSize-1)

	// Копируем элементы до индекса
	copy(newData[:index], array.data[:index])

	// Копируем элементы после индекса
	copy(newData[index:], array.data[index+1:])

	array.data = newData
	array.size = currentSize - 1
}

func MSWAP(array *Array, swapElement string, index int) {
	if array.data == nil {
		fmt.Println("Массив пуст")
		return
	}

	currentSize := array.size
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return
	}

	array.data[index] = swapElement
}

func MGET(array *Array, index int) string {
	if array.data == nil {
		fmt.Println("Массив пуст")
		return ""
	}

	currentSize := array.size
	if index >= currentSize {
		fmt.Println("Индекс вне массива")
		return ""
	}

	return array.data[index]
}

func MPRINT(array *Array) {
	if array.data == nil {
		fmt.Println("Массив пуст")
		return
	}

	for i := 0; i < array.size; i++ {
		fmt.Printf("%s index = %d\n", array.data[i], i)
	}
}
