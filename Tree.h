#pragma once
enum class Color{
    RED,
    BLACK
};

class RBNode{
    public:
        int data;
        Color color;
        RBNode* parent;
        RBNode* left;
        RBNode* right;
};

class RBTree{
    public:
        RBNode* root;
};

void leftRotate(RBTree* tree, RBNode* x);
void rightRotate(RBTree* tree, RBNode* x);
void fixAdd(RBTree* tree, RBNode* z);
void addRBNode(RBTree* tree, int value);
RBNode* getRBNode(RBTree* tree, int value);
RBNode* treeMinimum(RBTree* tree, RBNode* RBNode);
void deleteFix(RBTree* tree, RBNode* x);
void transplant(RBTree* tree, RBNode* u, RBNode* v);
void delRBNode(RBTree* tree, int value);
void printTree(RBNode* RBNode, int space = 0, int gap = 5);
void print(RBTree* tree);