#pragma once
class LLNode{
    public:
        int data;
        LLNode* previous;
        LLNode* next;
};
class LList{
    public:
        LLNode* tail;
        LLNode* head;
};
void LCREATE(LList& new_LList, int data);
LLNode* LGETINDEX(LList& list, int index);
LLNode* LGETDATA(LList& list, int data);
void LADDTAIL(LList& list, int data);
void LADDHEAD(LList& list, int data);
void LADDAFTER(LList& list, LLNode* ptr, int data);
void LADDBEFORE(LList& list, LLNode* ptr, int data);
void LDELTAIL(LList& list);
void LDELHEAD(LList& list);
void LDELAFTER(LList& list, LLNode* ptr);
void LDELBEFORE(LList& list, LLNode* ptr);
void LDELDATA(LList& list, int data);
void LPRINT(LList& list);