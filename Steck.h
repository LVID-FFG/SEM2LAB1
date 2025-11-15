#pragma once
#include <string>

using namespace std;
class SNode{
    public:
        string data;
        SNode* next;
        
        // Конструктор
        SNode(const string& val, SNode* next_ptr = nullptr) : data(val), next(next_ptr) {}
};

class Stack{
    public:
        SNode* head;
        
        // Конструктор
        Stack() : head(nullptr) {}
        Stack(const Stack& other) : head(nullptr) {
            SNode* current = other.head;
            SNode* last = nullptr;
            while (current != nullptr) {
                SNode* newNode = new SNode(current->data);
                if (head == nullptr) {
                    head = newNode;
                } else {
                    last->next = newNode;
                }
                last = newNode;
                current = current->next;
            }
        }

        Stack& operator=(const Stack& other) {
            if (this != &other) {
                // Очищаем текущий список
                while (head != nullptr) {
                    SNode* temp = head;
                    head = head->next;
                    delete temp;
                }
                
                // Копируем элементы
                SNode* current = other.head;
                SNode* last = nullptr;
                while (current != nullptr) {
                    SNode* newNode = new SNode(current->data);
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
        ~Stack() {
            while (head != nullptr) {
                SNode* temp = head;
                head = head->next;
                delete temp;
            }
        }
};

void SCREATE(Stack& new_Stack, const string& data);
void SPUSH(Stack& list, const string& data);
void SPOP(Stack& list);
void SPRINT(Stack& list);