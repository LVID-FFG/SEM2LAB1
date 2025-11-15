#include <iostream>
#include <string>
#include "FList.h"

using namespace std;

void FCREATE(FList& new_FList, const string& data){
    new_FList.head = new FLNode(data, nullptr);
}

FLNode* FGETINDEX(FList& list, int index){
    FLNode* Address = list.head;
    int currentIndex = 0;
    
    while (Address != nullptr) {
        if (currentIndex == index) {
            return Address;
        }
        Address = Address->next;
        currentIndex++;
    }
    
    cout << "Элемента с таким индексом нет" << endl;
    exit(1);
}

FLNode* FGETFLNodePARENT(FList& list, FLNode* target_FLNode){
    if (list.head == nullptr || list.head == target_FLNode) {
        return list.head;
    }
    
    FLNode* Address = list.head;
    while (Address->next != nullptr) {
        if (Address->next == target_FLNode) {
            return Address;
        }
        Address = Address->next;
    }
    
    cout << "Узел отсутствует в списке" << endl;
    exit(1);
}

FLNode* FGETDATA(FList& list, const string& data){
    FLNode* Address = list.head;
    while (Address != nullptr) {
        if (Address->data == data) {
            return Address;
        }
        Address = Address->next;
    }
    
    cout << "Узел отсутствует в списке" << endl;
    exit(1);
}

void FADDTAIL(FList& list, const string& data){
    FLNode* new_FLNode = new FLNode(data, nullptr);
    
    if (list.head == nullptr) {
        list.head = new_FLNode;
        return;
    }
    
    FLNode* Address = list.head;
    while (Address->next != nullptr) {
        Address = Address->next;
    }
    Address->next = new_FLNode;
}

void FADDHEAD(FList& list, const string& data){
    FLNode* new_FLNode = new FLNode(data, list.head);
    list.head = new_FLNode;
}

void FADDAFTER(FLNode* ptr, const string& data){
    FLNode* new_FLNode = new FLNode(data, ptr->next);
    ptr->next = new_FLNode;
}

void FADDBEFORE(FList& list, FLNode* ptr, const string& data){
    if (list.head == ptr){
        FADDHEAD(list, data);
        return;
    }
    
    FLNode* parent = FGETFLNodePARENT(list, ptr);
    FLNode* new_FLNode = new FLNode(data, ptr);
    parent->next = new_FLNode;
}

void FDELTAIL(FList& list){
    if (list.head == nullptr) return;
    
    if (list.head->next == nullptr) {
        delete list.head;
        list.head = nullptr;
        return;
    }
    
    FLNode* Address = list.head;
    while (Address->next->next != nullptr) {
        Address = Address->next;
    }
    
    delete Address->next;
    Address->next = nullptr;
}

void FDELHEAD(FList& list){
    if (list.head == nullptr) return;
    
    FLNode* Address = list.head->next;
    delete list.head;
    list.head = Address;
}

void FDELAFTER(FLNode* ptr){
    if (ptr->next == nullptr) return;
    
    FLNode* Address = ptr->next->next;
    delete ptr->next;
    ptr->next = Address;
}

void FDELBEFORE(FList& list, FLNode* ptr){
    if (list.head == ptr){
        cout << "Элемент первый в списке" << endl;
        return;
    }
    
    if (list.head->next == ptr){
        FDELHEAD(list);
        return;
    }
    
    FLNode* grandParent = FGETFLNodePARENT(list, FGETFLNodePARENT(list, ptr));
    FLNode* parent = grandParent->next;
    grandParent->next = ptr;
    delete parent;
}

void FDELDATA(FList& list, const string& data){
    if (list.head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    if (list.head->data == data){
        FDELHEAD(list);
        return;
    }

    FLNode* Address = list.head;
    while (Address->next != nullptr) {
        if (Address->next->data == data) {
            FDELAFTER(Address);
            return;
        }
        Address = Address->next;
    }
    
    cout << "Элемента нет в списке" << endl;
}

bool isListValid(FList& list) {
    if (list.head == nullptr) return true;
    
    FLNode* slow = list.head;
    FLNode* fast = list.head;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            cout << "DEBUG: Cycle detected in list!" << endl;
            return false;
        }
    }
    return true;
}

void FPRINT(FList& list){
    FLNode* Address = list.head;
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