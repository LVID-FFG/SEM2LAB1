#pragma once
#include <string>

using namespace std;

class FLNode{
    public:
        string data;
        FLNode* next;
        
        // Конструктор
        FLNode(const string& val, FLNode* next_ptr = nullptr) : data(val), next(next_ptr) {}
};

class FList{
    public:
        FLNode* head;
        
        // Конструктор
        FList() : head(nullptr) {}
        FList(const FList& other) : head(nullptr) {
            FLNode* current = other.head;
            FLNode* last = nullptr;
            while (current != nullptr) {
                FLNode* newNode = new FLNode(current->data);
                if (head == nullptr) {
                    head = newNode;
                } else {
                    last->next = newNode;
                }
                last = newNode;
                current = current->next;
            }
        }

        FList& operator=(const FList& other) {
            if (this != &other) {
                // Очищаем текущий список
                while (head != nullptr) {
                    FLNode* temp = head;
                    head = head->next;
                    delete temp;
                }
                
                // Копируем элементы
                FLNode* current = other.head;
                FLNode* last = nullptr;
                while (current != nullptr) {
                    FLNode* newNode = new FLNode(current->data);
                    if (head == nullptr) {
                        head = newNode;
                    } else {
                        last->next = newNode;
                    }
                    last = newNode;
                    current = current->next;
                }
            }
            return *this;
        }
        // Деструктор
        ~FList() {
            while (head != nullptr) {
                FLNode* temp = head;
                head = head->next;
                delete temp;
            }
        }
};

void FCREATE(FList& new_FList, const string& data);
FLNode* FGETINDEX(FList& list, int index);
FLNode* FGETDATA(FList& list, const string& data);
void FADDTAIL(FList& list, const string& data);
void FADDHEAD(FList& list, const string& data);
void FADDAFTER(FLNode* ptr, const string& data);
void FADDBEFORE(FList& list, FLNode* ptr, const string& data);
void FDELTAIL(FList& list);
void FDELHEAD(FList& list);
void FDELAFTER(FLNode* ptr);
void FDELBEFORE(FList& list, FLNode* ptr);
void FDELDATA(FList& list, const string& data);
void FPRINT(FList& list);