#pragma once
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

void QCREATE(Queue& new_Queue, int data);
void QPUSH(Queue& list, int data);
void QPOP(Queue& list);
void QPRINT(Queue& list);