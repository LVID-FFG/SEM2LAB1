#pragma once
#include <string>

using namespace std;

class Array{
public:
    string* data;
    size_t size;
    
    // Конструктор по умолчанию - создает пустой массив
    Array() : data(nullptr), size(0) {}
    
    // Конструктор копирования
    Array(const Array& other) : size(other.size) {
        if (other.data == nullptr) {
            data = nullptr;
        } else {
            data = new string[size];
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
    }
    
    // Оператор присваивания
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;  // Обновляем размер
            
            if (other.data == nullptr) {
                data = nullptr;
            } else {
                data = new string[size];
                for (size_t i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }
    
    // Деструктор
    ~Array() {
        delete[] data;
    }
};

void MSIZE(Array& array);
void MPUSHEND(Array& array, const string& a);
void MPUSH(Array& array, const string& a, int index);
void MDEL(Array& array, int index);
void MSWAP(Array& array, const string& swap_element, int index);
string MGET(Array& array, int index);
void MPRINT(Array& array);