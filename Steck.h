#pragma once

class SNode{
    public:
        int data;
        SNode* next;
};

class Stack{
    public:
        SNode* head;
};

void SCREATE(Stack& new_Stack, int data);
void SPUSH(Stack& list, int data);
void SPOP(Stack& list);
void SPRINT(Stack& list);