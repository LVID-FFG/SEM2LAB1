#include <iostream>

using namespace std;

class SNode{
    public:
        int data;
        SNode* next;
};

class Stack{
    public:
        SNode* head;
};

void SCREATE(Stack& new_Stack, int data){
    new_Stack.head = new SNode{data, nullptr};
}

void SPUSH(Stack& list, int data){
    SNode* new_SNode = new SNode{data, list.head};
    list.head = new_SNode;
}

void SPOP(Stack& list){
    SNode* Address = list.head -> next;
    delete list.head;
    list.head = Address;
}

void SPRINT(Stack& list){
    SNode* Address = list.head;
    while (Address != nullptr){
        cout << Address->data << endl;
        Address = Address->next;
    }
}
