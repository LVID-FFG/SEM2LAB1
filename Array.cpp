#include <iostream>

using namespace std;

class Array{
    public:
        int* data;
        int* end;
};

Array MCREATE(){
    Array array;
    array.data = new int;
    array.end = array.data;
    return array;
}

void MSIZE(Array& array){
    cout << "Size = "<< array.end - array.data << endl;
}

void MPUSHEND(Array& array, int a){
    size_t current_size = array.end - array.data;
    int* new_data = new int[current_size + 1];
    // Копируем старые данные
    for (size_t i = 0; i < current_size; i++) {
        new_data[i] = array.data[i];
    }
    // Добавляем новый элемент
    new_data[current_size] = a;
    
    // Освобождаем старую память и обновляем указатели
    delete[] array.data;
    array.data = new_data;
    array.end = array.data + current_size + 1;
}


void MPUSH(Array& array, int a, int index){
    size_t current_size = array.end - array.data;
    
    if (index >= current_size) {
        // Если индекс за пределами текущего размера - добавляем нули
        for (size_t i = 0; i < index - current_size; i++) {
            MPUSHEND(array, 0);
        }
        MPUSHEND(array, a);
    } else {
        // Вставка в середину массива
        MPUSHEND(array, 0); // Добавляем место в конце
        
        // Сдвигаем элементы вправо
        for (size_t i = current_size; i > index; i--) {
            array.data[i] = array.data[i - 1];
        }
        // Вставляем новый элемент
        array.data[index] = a;
    }
}

void MDEL (Array& array, int index){
    size_t current_size = array.end - array.data;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return;
    }else{
        index++;
        for(;index < current_size; index++) {
            array.data[index-1] = array.data[index];
        }
        array.end--;
    }
    if (array.data == array.end){
        delete array.data;
    }
}

void MSWAP (Array& array, int swap_element ,int index){
    size_t current_size = array.end - array.data;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return;
    }else{
        array.data[index] = swap_element;
    }
}

int MGET(Array& array, int index){
    size_t current_size = array.end - array.data;
    if (index >= current_size){
        cout << "Индекс вне массива" << endl;
        return 0;
    }else{
        return array.data[index];
    }
}

void MPRINT(Array& array){
    size_t i = 0;
    while (&array.data[i] != array.end){
        cout  << array.data[i] << " index = "<< i << endl;
        i++;
    }
}

