#include <iostream>
#include <string>
#include "LList.h"
using namespace std;

void LCREATE(LList& new_LList, const string& data){
    new_LList.head = new LLNode(data, nullptr, nullptr);
    new_LList.tail = new_LList.head;
}

LLNode* LGETINDEX(LList& list, int index){
    LLNode* Address = list.head;
    while(1){
        if (index == 0){
            return Address;
        }
        if(Address -> next == nullptr){
            if (index != 0){
                cout << "Элемента с таким индексом нет" << endl;
                exit(0);
            }
            return Address;
        }
        else{
            Address = Address -> next;
            index--;
        }
    }
}

LLNode* LGETDATA(LList& list, const string& data){
    LLNode* Address = list.head;
    while(1){
        if (Address -> data == data){
            return Address;
        }
        if(Address -> next == nullptr){
            if (Address -> data != data){
                cout << "Узел отсутствует в списке" << endl;
                exit(0);
            }
            return Address;
        }
        else{
            Address = Address -> next;
        }
    }
}


void LADDTAIL(LList& list, const string& data){
    LLNode* new_LLNode = new LLNode(data, list.tail, nullptr);
    list.tail -> next = new_LLNode;
    list.tail = new_LLNode;
}


void LADDHEAD(LList& list, const string& data){
    LLNode* new_LLNode = new LLNode(data, nullptr, list.head);
    
    // Обновляем previous у старой головы
    if (list.head != nullptr) {
        list.head->previous = new_LLNode;
    }
    
    list.head = new_LLNode;
    
    // Если список был пуст (tail = nullptr), обновляем tail
    if (list.tail == nullptr) {
        list.tail = new_LLNode;
    }
}

void LADDAFTER(LList& list, LLNode* ptr, const string& data){
    if (ptr -> next == nullptr){
        LADDTAIL(list, data);
        return;
    }
    LLNode* new_LLNode = new LLNode(data, ptr, ptr -> next);
    ptr -> next = new_LLNode;
    new_LLNode -> next -> previous = new_LLNode;
}

void LADDBEFORE(LList& list, LLNode* ptr, const string& data){
    if (list.head == ptr){
        LADDHEAD(list, data);
        return;
    }

    LLNode* new_LLNode = new LLNode(data, ptr->previous, ptr);
    
    // Обновляем указатель next у предыдущего узла
    ptr->previous->next = new_LLNode;
    
    // Обновляем указатель previous у текущего узла
    ptr->previous = new_LLNode;
}

//--------------------------------------

void LDELTAIL(LList& list){
    LLNode* Address = list.tail -> previous;
    delete list.tail;
    list.tail = Address;
    Address -> next = nullptr;
}


void LDELHEAD(LList& list){
    LLNode* Address = list.head -> next;
    delete list.head;
    list.head = Address;
}

void LDELAFTER(LList& list, LLNode* ptr){
    if (ptr -> next == nullptr) return;
    if (ptr -> next == list.tail) {
        LDELTAIL(list);
        return;
    }
    LLNode* Address = ptr -> next -> next;
    delete ptr -> next;
    ptr -> next = Address;
    Address -> previous = ptr;
}

void LDELBEFORE(LList& list, LLNode* ptr){
    if (list.head == ptr){
        cout << "Элемент первый в списке" << endl;
        return;
    }
    if (ptr -> previous == list.head) {
        LDELHEAD(list);
        return;
    }
    LLNode* Address = ptr -> previous -> previous;
    delete ptr -> previous;
    ptr -> previous = Address;
    Address -> next = ptr;
}

void LDELDATA(LList& list, const string& data){
    LLNode* Address = list.head;
    if (Address -> data == data){
        LDELHEAD(list);
        return;
    }

    while(1){

        if(Address -> next == nullptr){
            if (Address -> data == data){
                LDELTAIL(list);
                return;
            }
            cout << "Элемента нет в списке" << endl;
            return;
        }

        if (Address -> data == data){
            Address -> previous -> next = Address -> next;
            Address -> next -> previous = Address -> previous;
            delete Address;
            return;
        }

        Address = Address -> next;
    }
}

void LPRINT(LList& list){
    LLNode* Address = list.head;
    while (1){
        if(Address -> next == nullptr){
            cout << Address -> data << endl;
            return;
        }
        else{
            cout << Address -> data << endl;
            Address = Address -> next;
        }
    }
}