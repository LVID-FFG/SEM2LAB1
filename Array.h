//#ifndef ARRAY_H
#pragma once
//#define ARRAY_H

class Array {
public:
    int* data;
    int* end;
};

// Прототипы функций
Array MCREATE();
void MSIZE(Array& array);
void MPUSHEND(Array& array, int a);
void MPUSH(Array& array, int a, int index);
void MDEL(Array& array, int index);
void MSWAP(Array& array, int swap_element, int index);
int MGET(Array& array, int index);
void MPRINT(Array& array);

//#endif // ARRAY_H