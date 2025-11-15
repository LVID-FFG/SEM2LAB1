package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// ==================== СИСТЕМА УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ ====================

type Database struct {
	arrays   map[string]*Array
	flists   map[string]*FList
	llists   map[string]*LList
	queues   map[string]*Queue
	stacks   map[string]*Stack
	filename string
}

func NewDatabase(file string) *Database {
	return &Database{
		arrays:   make(map[string]*Array),
		flists:   make(map[string]*FList),
		llists:   make(map[string]*LList),
		queues:   make(map[string]*Queue),
		stacks:   make(map[string]*Stack),
		filename: file,
	}
}

// ==================== СЕРИАЛИЗАЦИЯ ====================

func (db *Database) SaveToFile() {
	file, err := os.Create(db.filename)
	if err != nil {
		fmt.Printf("Ошибка, невозможно открыть файл %s\n", db.filename)
		return
	}
	defer file.Close()

	writer := bufio.NewWriter(file)

	// Сохраняем массивы
	writer.WriteString(fmt.Sprintf("ARRAYS %d\n", len(db.arrays)))
	for name, arr := range db.arrays {
		writer.WriteString(fmt.Sprintf("ARRAY %s\n", name))
		writer.WriteString(fmt.Sprintf("%d\n", arr.size))

		if arr.size > 0 {
			for i := 0; i < arr.size; i++ {
				writer.WriteString(fmt.Sprintf("%s\n", arr.data[i]))
			}
		}
	}

	// Сохраняем односвязные списки
	writer.WriteString(fmt.Sprintf("FLISTS %d\n", len(db.flists)))
	for name, flist := range db.flists {
		writer.WriteString(fmt.Sprintf("FLIST %s\n", name))

		// Подсчитываем элементы
		count := 0
		current := flist.head
		for current != nil {
			count++
			current = current.next
		}

		writer.WriteString(fmt.Sprintf("%d\n", count))
		current = flist.head
		for current != nil {
			writer.WriteString(fmt.Sprintf("%s\n", current.data))
			current = current.next
		}
	}

	// Сохраняем двусвязные списки
	writer.WriteString(fmt.Sprintf("LLISTS %d\n", len(db.llists)))
	for name, llist := range db.llists {
		writer.WriteString(fmt.Sprintf("LLIST %s\n", name))

		// Подсчитываем элементы
		count := 0
		current := llist.head
		for current != nil {
			count++
			current = current.next
		}

		writer.WriteString(fmt.Sprintf("%d\n", count))
		current = llist.head
		for current != nil {
			writer.WriteString(fmt.Sprintf("%s\n", current.data))
			current = current.next
		}
	}

	// Сохраняем стеки
	writer.WriteString(fmt.Sprintf("STACKS %d\n", len(db.stacks)))
	for name, stack := range db.stacks {
		writer.WriteString(fmt.Sprintf("STACK %s\n", name))

		// Подсчитываем элементы
		count := 0
		current := stack.head
		for current != nil {
			count++
			current = current.next
		}

		// Сохраняем в обратном порядке для стека
		writer.WriteString(fmt.Sprintf("%d\n", count))
		var temp []string
		current = stack.head
		for current != nil {
			temp = append(temp, current.data)
			current = current.next
		}
		// Записываем от дна к вершине
		for i := len(temp) - 1; i >= 0; i-- {
			writer.WriteString(fmt.Sprintf("%s\n", temp[i]))
		}
	}

	// Сохраняем очереди
	writer.WriteString(fmt.Sprintf("QUEUES %d\n", len(db.queues)))
	for name, queue := range db.queues {
		writer.WriteString(fmt.Sprintf("QUEUE %s\n", name))

		// Подсчитываем элементы
		count := 0
		current := queue.head
		for current != nil {
			count++
			current = current.next
		}

		writer.WriteString(fmt.Sprintf("%d\n", count))
		current = queue.head
		for current != nil {
			writer.WriteString(fmt.Sprintf("%s\n", current.data))
			current = current.next
		}
	}

	writer.Flush()
	fmt.Printf("База данных сохранена %s\n", db.filename)
}

func (db *Database) LoadFromFile() {
	file, err := os.Open(db.filename)
	if err != nil {
		fmt.Printf("Файл %s ненайден, создан новый файл\n", db.filename)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		section := scanner.Text()
		parts := strings.Fields(section)
		if len(parts) < 2 {
			continue
		}

		switch parts[0] {
		case "ARRAYS":
			count, _ := strconv.Atoi(parts[1])
			for i := 0; i < count; i++ {
				if !scanner.Scan() {
					break
				}
				typeLine := strings.Fields(scanner.Text())
				if len(typeLine) < 3 {
					continue
				}

				name := typeLine[2]
				if !scanner.Scan() {
					break
				}
				size, _ := strconv.Atoi(scanner.Text())

				arr := NewArray()
				arr.size = size
				if size > 0 {
					arr.data = make([]string, size)
					for j := 0; j < size; j++ {
						if !scanner.Scan() {
							break
						}
						arr.data[j] = scanner.Text()
					}
				}
				db.arrays[name] = arr
			}

		case "FLISTS":
			count, _ := strconv.Atoi(parts[1])
			for i := 0; i < count; i++ {
				if !scanner.Scan() {
					break
				}
				typeLine := strings.Fields(scanner.Text())
				if len(typeLine) < 3 {
					continue
				}

				name := typeLine[2]
				if !scanner.Scan() {
					break
				}
				elementCount, _ := strconv.Atoi(scanner.Text())

				flist := NewFList()
				if elementCount > 0 {
					var elements []string
					for j := 0; j < elementCount; j++ {
						if !scanner.Scan() {
							break
						}
						elements = append(elements, scanner.Text())
					}
					if len(elements) > 0 {
						FCREATE(flist, elements[0])
						for j := 1; j < len(elements); j++ {
							FADDTAIL(flist, elements[j])
						}
					}
				}
				db.flists[name] = flist
			}

		case "LLISTS":
			count, _ := strconv.Atoi(parts[1])
			for i := 0; i < count; i++ {
				if !scanner.Scan() {
					break
				}
				typeLine := strings.Fields(scanner.Text())
				if len(typeLine) < 3 {
					continue
				}

				name := typeLine[2]
				if !scanner.Scan() {
					break
				}
				elementCount, _ := strconv.Atoi(scanner.Text())

				llist := NewLList()
				if elementCount > 0 {
					if !scanner.Scan() {
						break
					}
					data := scanner.Text()
					LCREATE(llist, data)

					for j := 1; j < elementCount; j++ {
						if !scanner.Scan() {
							break
						}
						data = scanner.Text()
						LADDTAIL(llist, data)
					}
				}
				db.llists[name] = llist
			}

		case "STACKS":
			count, _ := strconv.Atoi(parts[1])
			for i := 0; i < count; i++ {
				if !scanner.Scan() {
					break
				}
				typeLine := strings.Fields(scanner.Text())
				if len(typeLine) < 3 {
					continue
				}

				name := typeLine[2]
				if !scanner.Scan() {
					break
				}
				elementCount, _ := strconv.Atoi(scanner.Text())

				stack := NewStack()
				if elementCount > 0 {
					var temp []string
					for j := 0; j < elementCount; j++ {
						if !scanner.Scan() {
							break
						}
						temp = append(temp, scanner.Text())
					}
					// Восстанавливаем стек
					SCREATE(stack, temp[0])
					for j := 1; j < elementCount; j++ {
						SPUSH(stack, temp[j])
					}
				}
				db.stacks[name] = stack
			}

		case "QUEUES":
			count, _ := strconv.Atoi(parts[1])
			for i := 0; i < count; i++ {
				if !scanner.Scan() {
					break
				}
				typeLine := strings.Fields(scanner.Text())
				if len(typeLine) < 3 {
					continue
				}

				name := typeLine[2]
				if !scanner.Scan() {
					break
				}
				elementCount, _ := strconv.Atoi(scanner.Text())

				queue := NewQueue()
				if elementCount > 0 {
					if !scanner.Scan() {
						break
					}
					data := scanner.Text()
					QCREATE(queue, data)

					for j := 1; j < elementCount; j++ {
						if !scanner.Scan() {
							break
						}
						data = scanner.Text()
						QPUSH(queue, data)
					}
				}
				db.queues[name] = queue
			}
		}
	}

	fmt.Printf("База данных загружена из %s\n", db.filename)
}

// ==================== ОБРАБОТКА ЗАПРОСОВ ====================

func (db *Database) ExecuteQuery(query string) string {
	tokens := strings.Fields(query)
	if len(tokens) == 0 {
		return "Ошибка, запрос пуст"
	}

	command := strings.ToUpper(tokens[0])

	// ==================== МАССИВЫ ====================
	if command == "MCREATE" && len(tokens) == 2 {
		name := tokens[1]
		db.arrays[name] = NewArray()
		return "Запрос успешно выполнен"
	} else if command == "MSIZE" && len(tokens) == 2 {
		name := tokens[1]
		if arr, exists := db.arrays[name]; exists {
			return strconv.Itoa(arr.size)
		}
		return "Массив ненайден"
	} else if command == "MPUSHEND" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if _, exists := db.arrays[name]; !exists {
			db.arrays[name] = NewArray()
		}
		MPUSHEND(db.arrays[name], value)
		return value
	} else if command == "MPUSH" && len(tokens) == 4 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		if _, exists := db.arrays[name]; !exists {
			db.arrays[name] = NewArray()
		}
		MPUSH(db.arrays[name], value, index)
		return value
	} else if command == "MDEL" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		MDEL(db.arrays[name], index)
		return "Запрос успешно выполнен"
	} else if command == "MSWAP" && len(tokens) == 4 {
		name := tokens[1]
		value := tokens[2]
		index, _ := strconv.Atoi(tokens[3])
		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		MSWAP(db.arrays[name], value, index)
		return "Запрос успешно выполнен"
	} else if command == "MGET" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		if _, exists := db.arrays[name]; !exists {
			return "Массив не найден"
		}
		return MGET(db.arrays[name], index)
	} else if command == "MPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if arr, exists := db.arrays[name]; exists {
			MPRINT(arr)
			return "Запрос успешно выполнен"
		}
		return "Массив не найден"
	} else if command == "FCREATE" && len(tokens) == 3 { // ==================== ОДНОСВЯЗНЫЕ СПИСКИ ====================
		name := tokens[1]
		value := tokens[2]
		flist := NewFList()
		FCREATE(flist, value)
		db.flists[name] = flist
		return value
	} else if command == "FGETINDEX" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		if flist, exists := db.flists[name]; exists {
			node := FGETINDEX(flist, index)
			if node != nil {
				return node.data
			}
			return "Элемента с таким индексом нет"
		}
		return "Список не найден"
	} else if command == "FGETDATA" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if flist, exists := db.flists[name]; exists {
			node := FGETDATA(flist, value)
			if node != nil {
				return node.data
			}
			return "Узел отсутствует в списке"
		}
		return "Список не найден"
	} else if command == "FADDTAIL" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if flist, exists := db.flists[name]; exists {
			FADDTAIL(flist, value)
			return value
		}
		return "Список не найден"
	} else if command == "FADDHEAD" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if flist, exists := db.flists[name]; exists {
			FADDHEAD(flist, value)
			return value
		}
		return "Список не найден"
	} else if command == "FADDAFTERINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			if targetNode != nil {
				FADDAFTER(targetNode, value)
				return value
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "FADDAFTERDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue := tokens[2]
		value := tokens[3]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			if targetNode != nil {
				FADDAFTER(targetNode, value)
				return value
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "FADDBEFOREINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			if targetNode != nil {
				FADDBEFORE(flist, targetNode, value)
				return value
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "FADDBEFOREDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue := tokens[2]
		value := tokens[3]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			if targetNode != nil {
				FADDBEFORE(flist, targetNode, value)
				return value
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "FDELTAIL" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FDELTAIL(flist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "FDELHEAD" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FDELHEAD(flist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "FDELAFTERINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			if targetNode != nil {
				FDELAFTER(targetNode)
				return "Запрос успешно выполнен"
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "FDELAFTERDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue := tokens[2]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			if targetNode != nil {
				FDELAFTER(targetNode)
				return "Запрос успешно выполнен"
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "FDELBEFOREINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETINDEX(flist, targetIndex)
			if targetNode != nil {
				FDELBEFORE(flist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "FDELBEFOREDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue := tokens[2]
		if flist, exists := db.flists[name]; exists {
			targetNode := FGETDATA(flist, targetValue)
			if targetNode != nil {
				FDELBEFORE(flist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "FDELDATA" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if flist, exists := db.flists[name]; exists {
			FDELDATA(flist, value)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "FPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if flist, exists := db.flists[name]; exists {
			FPRINT(flist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "LCREATE" && len(tokens) == 3 { // ==================== ДВУСВЯЗНЫЕ СПИСКИ ====================
		name := tokens[1]
		value := tokens[2]
		llist := NewLList()
		LCREATE(llist, value)
		db.llists[name] = llist
		return value
	} else if command == "LGETINDEX" && len(tokens) == 3 {
		name := tokens[1]
		index, _ := strconv.Atoi(tokens[2])
		if llist, exists := db.llists[name]; exists {
			node := LGETINDEX(llist, index)
			if node != nil {
				return node.data
			}
			return "Элемента с таким индексом нет"
		}
		return "Список не найден"
	} else if command == "LGETDATA" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if llist, exists := db.llists[name]; exists {
			node := LGETDATA(llist, value)
			if node != nil {
				return node.data
			}
			return "Узел отсутствует в списке"
		}
		return "Список не найден"
	} else if command == "LADDTAIL" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if llist, exists := db.llists[name]; exists {
			LADDTAIL(llist, value)
			return value
		}
		return "Список не найден"
	} else if command == "LADDHEAD" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if llist, exists := db.llists[name]; exists {
			LADDHEAD(llist, value)
			return value
		}
		return "Список не найден"
	} else if command == "LADDAFTERINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			if targetNode != nil {
				LADDAFTER(llist, targetNode, value)
				return value
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "LADDAFTERDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue := tokens[2]
		value := tokens[3]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			if targetNode != nil {
				LADDAFTER(llist, targetNode, value)
				return value
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "LADDBEFOREINDEX" && len(tokens) == 4 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		value := tokens[3]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			if targetNode != nil {
				LADDBEFORE(llist, targetNode, value)
				return value
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "LADDBEFOREDATA" && len(tokens) == 4 {
		name := tokens[1]
		targetValue := tokens[2]
		value := tokens[3]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			if targetNode != nil {
				LADDBEFORE(llist, targetNode, value)
				return value
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "LDELTAIL" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LDELTAIL(llist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "LDELHEAD" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LDELHEAD(llist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "LDELAFTERINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			if targetNode != nil {
				LDELAFTER(llist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "LDELAFTERDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue := tokens[2]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			if targetNode != nil {
				LDELAFTER(llist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "LDELBEFOREINDEX" && len(tokens) == 3 {
		name := tokens[1]
		targetIndex, _ := strconv.Atoi(tokens[2])
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETINDEX(llist, targetIndex)
			if targetNode != nil {
				LDELBEFORE(llist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Индекс не найден"
		}
		return "Список не найден"
	} else if command == "LDELBEFOREDATA" && len(tokens) == 3 {
		name := tokens[1]
		targetValue := tokens[2]
		if llist, exists := db.llists[name]; exists {
			targetNode := LGETDATA(llist, targetValue)
			if targetNode != nil {
				LDELBEFORE(llist, targetNode)
				return "Запрос успешно выполнен"
			}
			return "Значение не найдено"
		}
		return "Список не найден"
	} else if command == "LDELDATA" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if llist, exists := db.llists[name]; exists {
			LDELDATA(llist, value)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "LPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if llist, exists := db.llists[name]; exists {
			LPRINT(llist)
			return "Запрос успешно выполнен"
		}
		return "Список не найден"
	} else if command == "QCREATE" && len(tokens) == 3 { // ==================== ОЧЕРЕДИ ====================
		name := tokens[1]
		value := tokens[2]
		queue := NewQueue()
		QCREATE(queue, value)
		db.queues[name] = queue
		return value
	} else if command == "QPUSH" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if queue, exists := db.queues[name]; exists {
			QPUSH(queue, value)
			return value
		}
		return "Очередь не найдена"
	} else if command == "QPOP" && len(tokens) == 2 {
		name := tokens[1]
		if queue, exists := db.queues[name]; exists && queue.head != nil {
			value := queue.head.data
			QPOP(queue)
			return value
		}
		return "Очередь не существует или пуста"
	} else if command == "QPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if queue, exists := db.queues[name]; exists {
			QPRINT(queue)
			return "Запрос успешно выполнен"
		}
		return "Очередь не найдена"
	} else if command == "SCREATE" && len(tokens) == 3 { // ==================== СТЕКИ ====================
		name := tokens[1]
		value := tokens[2]
		if _, exists := db.stacks[name]; exists {
			return "Стэк с таким именем уже существует"
		}
		stack := NewStack()
		SCREATE(stack, value)
		db.stacks[name] = stack
		return value
	} else if command == "SPUSH" && len(tokens) == 3 {
		name := tokens[1]
		value := tokens[2]
		if stack, exists := db.stacks[name]; exists {
			SPUSH(stack, value)
			return value
		}
		return "Стэк не найден"
	} else if command == "SPOP" && len(tokens) == 2 {
		name := tokens[1]
		if stack, exists := db.stacks[name]; exists && stack.head != nil {
			value := stack.head.data
			SPOP(stack)
			return value
		}
		return "Стэк пуст или не существует"
	} else if command == "SPRINT" && len(tokens) == 2 {
		name := tokens[1]
		if stack, exists := db.stacks[name]; exists {
			SPRINT(stack)
			return "Запрос успешно выполнен"
		}
		return "Стэк не найден"
	} else {
		return "Ошибка, неизвестная команда или неправильные аргументы"
	}
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
}

func main() {
	if len(os.Args) < 5 {
		printUsage()
		return
	}

	var filename, query string

	for i := 1; i < len(os.Args); i++ {
		arg := os.Args[i]
		switch arg {
		case "--file":
			if i+1 < len(os.Args) {
				filename = os.Args[i+1]
				i++
			}
		case "--query":
			if i+1 < len(os.Args) {
				query = os.Args[i+1]
				i++
			}
		case "--help":
			printUsage()
			return
		}
	}

	if filename == "" || query == "" {
		printUsage()
		return
	}

	// Создаем и загружаем базу данных
	db := NewDatabase(filename)
	db.LoadFromFile()

	// Выполняем запрос
	result := db.ExecuteQuery(query)
	fmt.Println(result)

	// Сохраняем изменения
	db.SaveToFile()
}
