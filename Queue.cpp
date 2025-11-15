#include <iostream>
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

void QCREATE(Queue& new_Queue, const string& data){
    new_Queue.head = new QNode(data, nullptr, nullptr);
    new_Queue.tail = new_Queue.head;
}

void QPUSH(Queue& list, const string& data){
    QNode* new_QNode = new QNode(data, list.tail, nullptr);
    list.tail -> next = new_QNode;
    list.tail = new_QNode;
}

void QPOP(Queue& list){
    QNode* Address = list.head -> next;
    delete list.head;
    list.head = Address;
}

void QPRINT(Queue& list){
    QNode* Address = list.head;
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