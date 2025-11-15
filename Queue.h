
#pragma once
#include <string>

using namespace std;

class QNode{
    public:
        string data;
        QNode* previous;
        QNode* next;
        
        QNode(const string& val, QNode* prev = nullptr, QNode* nxt = nullptr) 
            : data(val), previous(prev), next(nxt) {}
};

class Queue{
    public:
        QNode* tail;
        QNode* head;
        
        Queue() : tail(nullptr), head(nullptr) {}
        
        // Конструктор копирования
        Queue(const Queue& other) : tail(nullptr), head(nullptr) {
            if (other.head != nullptr) {
                QNode* currentOther = other.head;
                QNode* last = nullptr;
                
                // Копируем первый элемент
                head = new QNode(currentOther->data);
                last = head;
                currentOther = currentOther->next;
                
                // Копируем остальные элементы
                while (currentOther != nullptr) {
                    QNode* newNode = new QNode(currentOther->data, last);
                    last->next = newNode;
                    last = newNode;
                    currentOther = currentOther->next;
                }
                tail = last;
            }
        }
        
        // Оператор присваивания
        Queue& operator=(const Queue& other) {
            if (this != &other) {
                // Очищаем текущий список
                QNode* current = head;
                while (current != nullptr) {
                    QNode* next = current->next;
                    delete current;
                    current = next;
                }
                head = nullptr;
                tail = nullptr;
                
                // Копируем элементы из другого списка
                if (other.head != nullptr) {
                    QNode* currentOther = other.head;
                    QNode* last = nullptr;
                    
                    head = new QNode(currentOther->data);
                    last = head;
                    currentOther = currentOther->next;
                    
                    while (currentOther != nullptr) {
                        QNode* newNode = new QNode(currentOther->data, last);
                        last->next = newNode;
                        last = newNode;
                        currentOther = currentOther->next;
                    }
                    tail = last;
                }
            }
            return *this;
        }
        
        ~Queue() {
            QNode* current = head;
            while (current != nullptr) {
                QNode* next = current->next;
                delete current;
                current = next;
            }
        }
};

void QCREATE(Queue& new_Queue, const string& data);
void QPUSH(Queue& list, const string& data);
void QPOP(Queue& list);
void QPRINT(Queue& list);