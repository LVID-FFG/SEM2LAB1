#include <iostream>

using namespace std;

class QNode{
    public:
        int data;
        QNode* previous;
        QNode* next;
};

class Queue{
    public:
        QNode* tail;
        QNode* head;
};

void QCREATE(Queue& new_Queue, int data){
    new_Queue.head = new QNode{data, nullptr, nullptr};
    new_Queue.tail = new_Queue.head;
}

void QPUSH(Queue& list, int data){
    QNode* new_QNode = new QNode{data, list.tail, nullptr};
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