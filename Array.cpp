#include <iostream>
#include <string>
#include "Array.h"

using namespace std;

void MSIZE(Array& array){
    cout << "Size = " << array.size << endl;
}

void MPUSHEND(Array& array, const string& a){
    if (array.data == nullptr) {
        // Если массив пустой, создаем с одним элементом
        array.data = new string[1];
        array.data[0] = a;
        array.size = 1;  // Устанавливаем размер
        return;
    }
    
    size_t current_size = array.size;
    string* new_data = new string[current_size + 1];
    
    // Копируем старые данные
    for (size_t i = 0; i < current_size; i++) {
        new_data[i] = array.data[i];
    }
    
    // Добавляем новый элемент
    new_data[current_size] = a;
    
    // Освобождаем старую память и обновляем указатели
    delete[] array.data;
    array.data = new_data;
    array.size = current_size + 1;  // Обновляем размер
}

void MPUSH(Array& array, const string& a, int index){
    if (array.data == nullptr) {
        // Если массив пустой, создаем и добавляем элемент
        array.data = new string[1];
        array.data[0] = a;
        array.size = 1;  // Устанавливаем размер
        return;
    }
    
    size_t current_size = array.size;
    
    if (index >= current_size) {
        // Если индекс за пределами - добавляем в конец
        MPUSHEND(array, a);
    } else {
        // Вставка в середину массива
        string* new_data = new string[current_size + 1];
        
        // Копируем элементы до индекса
        for (size_t i = 0; i < index; i++) {
            new_data[i] = array.data[i];
        }
        
        // Вставляем новый элемент
        new_data[index] = a;
        
        // Копируем оставшиеся элементы
        for (size_t i = index; i < current_size; i++) {
            new_data[i + 1] = array.data[i];
        }
        
        delete[] array.data;
        array.data = new_data;
        array.size = current_size + 1;
    }
}

void MDEL(Array& array, int index){
    if (array.data == nullptr) {
        cout << "Массив пуст" << endl;
        return;
    }
    
    size_t current_size = array.size;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return;
    }
    
    if (current_size == 1) {
        // Если удаляем последний элемент
        delete[] array.data;
        array.data = nullptr;
        array.size = 0;  // Обнуляем размер
        return;
    }
    
    // Создаем новый массив на один элемент меньше
    string* new_data = new string[current_size - 1];
    
    // Копируем элементы до индекса
    for (size_t i = 0; i < index; i++) {
        new_data[i] = array.data[i];
    }
    
    // Копируем элементы после индекса
    for (size_t i = index + 1; i < current_size; i++) {
        new_data[i - 1] = array.data[i];
    }
    
    delete[] array.data;
    array.data = new_data;
    array.size = current_size - 1;
}

void MSWAP(Array& array, const string& swap_element, int index){
    if (array.data == nullptr) {
        cout << "Массив пуст" << endl;
        return;
    }
    
    size_t current_size = array.size;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return;
    }
    
    array.data[index] = swap_element;
}

string MGET(Array& array, int index){
    if (array.data == nullptr) {
        cout << "Массив пуст" << endl;
        return "";
    }
    
    size_t current_size = array.size;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return "";
    }
    
    return array.data[index];
}

void MPRINT(Array& array){
    if (array.data == nullptr) {
        cout << "Массив пуст" << endl;
        return;
    }
    
    for (size_t i = 0; i < array.size; i++) {
        cout << array.data[i] << " index = " << i << endl;
    }
}