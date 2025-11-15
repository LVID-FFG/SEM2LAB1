#pragma once
#include <string>

using namespace std;

class LLNode{
    public:
        string data;
        LLNode* previous;
        LLNode* next;
        
        LLNode(const string& val, LLNode* prev = nullptr, LLNode* nxt = nullptr) 
            : data(val), previous(prev), next(nxt) {}
};

class LList{
    public:
        LLNode* tail;
        LLNode* head;
        
        LList() : tail(nullptr), head(nullptr) {}
        
        // Конструктор копирования
        LList(const LList& other) : tail(nullptr), head(nullptr) {
            if (other.head != nullptr) {
                LLNode* currentOther = other.head;
                LLNode* last = nullptr;
                
                // Копируем первый элемент
                head = new LLNode(currentOther->data);
                last = head;
                currentOther = currentOther->next;
                
                // Копируем остальные элементы
                while (currentOther != nullptr) {
                    LLNode* newNode = new LLNode(currentOther->data, last);
                    last->next = newNode;
                    last = newNode;
                    currentOther = currentOther->next;
                }
                tail = last;
            }
        }
        
        // Оператор присваивания
        LList& operator=(const LList& other) {
            if (this != &other) {
                // Очищаем текущий список
                LLNode* current = head;
                while (current != nullptr) {
                    LLNode* next = current->next;
                    delete current;
                    current = next;
                }
                head = nullptr;
                tail = nullptr;
                
                // Копируем элементы из другого списка
                if (other.head != nullptr) {
                    LLNode* currentOther = other.head;
                    LLNode* last = nullptr;
                    
                    head = new LLNode(currentOther->data);
                    last = head;
                    currentOther = currentOther->next;
                    
                    while (currentOther != nullptr) {
                        LLNode* newNode = new LLNode(currentOther->data, last);
                        last->next = newNode;
                        last = newNode;
                        currentOther = currentOther->next;
                    }
                    tail = last;
                }
            }
            return *this;
        }
        
        ~LList() {
            LLNode* current = head;
            while (current != nullptr) {
                LLNode* next = current->next;
                delete current;
                current = next;
            }
        }
};

void LCREATE(LList& new_LList, const string& data);
LLNode* LGETINDEX(LList& list, int index);
LLNode* LGETDATA(LList& list, const string& data);
void LADDTAIL(LList& list, const string& data);
void LADDHEAD(LList& list, const string& data);
void LADDAFTER(LList& list, LLNode* ptr, const string& data);
void LADDBEFORE(LList& list, LLNode* ptr, const string& data);
void LDELTAIL(LList& list);
void LDELHEAD(LList& list);
void LDELAFTER(LList& list, LLNode* ptr);
void LDELBEFORE(LList& list, LLNode* ptr);
void LDELDATA(LList& list, const string& data);
void LPRINT(LList& list);