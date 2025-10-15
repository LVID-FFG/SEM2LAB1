#pragma once

class FLNode{
    public:
        int data;
        FLNode* next;
};
class FList{
    public:
        FLNode* head;
};

void FCREATE(FList& new_FList, int data);
FLNode* FGETINDEX(FList& list, int index);
FLNode* FGETFLNodePARENT(FList& list, FLNode* target_FLNode);
FLNode* FGETDATA(FList& list, int data);
void FADDTAIL(FList& list, int data);
void FADDHEAD(FList& list, int data);
void FADDAFTER(FLNode* ptr, int data);
void FADDBEFORE(FList& list, FLNode* ptr, int data);
void FDELTAIL(FList& list);
void FDELHEAD(FList& list);
void FDELAFTER(FLNode* ptr);
void FDELBEFORE(FList& list, FLNode* ptr);
void FDELDATA(FList& list, int data);
void FPRINT(FList& list);