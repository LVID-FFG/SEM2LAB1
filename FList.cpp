#include <iostream>

using namespace std;

class FLNode{
    public:
        int data;
        FLNode* next;
};

class FList{
    public:
        FLNode* head;
};

void FCREATE(FList& new_FList, int data){
    new_FList.head = new FLNode{data, nullptr};
}

FLNode* FGETINDEX(FList& list, int index){
    FLNode* Address = list.head;
    while(1){
        if (index == 0){
            return Address;
        }
        if(Address -> next == nullptr){
            if (index != 0){
                cout << "Элемента с таким индексом нет" << endl;
                exit(0);
            }
            return Address;
        }
        else{
            Address = Address -> next;
            index--;
        }
    }
}

FLNode* FGETFLNodePARENT(FList& list, FLNode* target_FLNode){
    FLNode* Address = list.head;
    while(1){
        if (Address -> next == target_FLNode){
            return Address;
        }
        if(Address -> next == nullptr){
            if (Address != target_FLNode){
                cout << "Узел отсутствует в списке" << endl;
                exit(0);
            }
            return Address;
        }
        else{
            Address = Address -> next;
        }
    }
}

FLNode* FGETDATA(FList& list, int data){
    FLNode* Address = list.head;
    while(1){
        if (Address -> data == data){
            return Address;
        }
        if(Address -> next == nullptr){
            if (Address -> data != data){
                cout << "Узел отсутствует в списке" << endl;
                exit(0);
            }
            return Address;
        }
        else{
            Address = Address -> next;
        }
    }
}

void FADDTAIL(FList& list, int data){
    FLNode* new_FLNode = new FLNode{data, nullptr};
    FLNode* Address = list.head;
    while(1){
        if(Address -> next == nullptr){
            Address -> next = new_FLNode;
            break;
        }
        else{
            Address = Address -> next;
        }
    }
}

void FADDHEAD(FList& list, int data){
    FLNode* new_FLNode = new FLNode{data, list.head};
    list.head = new_FLNode;
}

void FADDAFTER(FLNode* ptr, int data){
    FLNode* new_FLNode = new FLNode{data, ptr -> next};
    ptr -> next = new_FLNode;
}

void FADDBEFORE(FList& list, FLNode* ptr, int data){
    if (list.head == ptr){
        FADDHEAD(list, data);
        return;
    }
    FLNode* new_FLNode = new FLNode{data, ptr};
    FGETFLNodePARENT(list, ptr) -> next = new_FLNode;
}

//--------------------------------------

void FDELTAIL(FList& list){
    FLNode* Address = list.head;
    while(1){
        if(Address -> next -> next == nullptr){
            delete Address -> next -> next;
            Address -> next = nullptr;
            break;
        }
        else{
            Address = Address -> next;
        }
    }
}

void FDELHEAD(FList& list){
    FLNode* Address = list.head -> next;
    delete list.head;
    list.head = Address;
}

void FDELAFTER(FLNode* ptr){
    if (ptr -> next == nullptr) return;
    FLNode* Address = ptr -> next -> next;
    delete ptr -> next;
    ptr -> next = Address;
}

void FDELBEFORE(FList& list, FLNode* ptr){
    if (list.head == ptr){
        cout << "Элемент первый в списке" << endl;
        return;
    }
    if (list.head -> next == ptr){
        FDELHEAD(list);
        return;
    }
    FLNode* Address = FGETFLNodePARENT(list, FGETFLNodePARENT(list, ptr));
    delete FGETFLNodePARENT(list, ptr);
    Address -> next = ptr;
}

void FDELDATA(FList& list, int data){
    FLNode* Address = list.head;
    if (Address -> data == data){
        FDELHEAD(list);
        return;
    }

    FLNode* current = list.head;

    while(1){
        if(Address -> next == nullptr){
            cout << "Элемента нет в списке" << endl;
            return;
        }
        if (Address -> next -> data == data){
            FDELAFTER(Address);
            return;
        }
        Address = Address -> next;
    }
}

void FPRINT(FList& list){
    FLNode* Address = list.head;
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