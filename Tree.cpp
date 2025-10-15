#include <iostream>

using namespace std;

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

void leftRotate(RBTree* tree, RBNode* x){
    RBNode* y = x -> right;
    x -> right = y -> left;
    if (y -> left != nullptr){
        y -> left -> parent = x;
    }
    y -> parent = x-> parent;
    if (x -> parent == nullptr){
        tree -> root = y;
    }else{
        if(x == x -> parent -> left) x -> parent -> left = y;
        else x-> parent -> right = y;
    }
    y -> left = x;
    x -> parent = y;
}

void rightRotate(RBTree* tree, RBNode* x){
    RBNode* y = x -> left;
    x -> left = y -> right;
    if (y -> right != nullptr){
        y -> right -> parent = x;
    }
    y -> parent = x-> parent;
    if (x -> parent == nullptr){
        tree -> root = y;
    }else{
        if(x == x -> parent -> left) x -> parent -> left = y;
        else x-> parent -> right = y;
    }
    y -> right = x;
    x -> parent = y;
}

void fixAdd(RBTree* tree, RBNode* z){
    while (z->parent != nullptr && z->parent->color == Color::RED){
        
        // Проверяем, что дедушка существует
        if (z->parent->parent == nullptr) {
            break;
        }

        if(z->parent == z->parent->parent->left){
            RBNode* y = z->parent->parent->right;
            
            // Проверяем, существует ли дядя (y) и он красный
            if (y != nullptr && y->color == Color::RED){
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            }
            else{
                if (z == z->parent->right){
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rightRotate(tree, z->parent->parent);
            }
        }
        else{
            RBNode* y = z->parent->parent->left;
            
            // Проверяем, существует ли дядя (y) и он красный
            if (y != nullptr && y->color == Color::RED){
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            }
            else{
                if (z == z->parent->left){
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                leftRotate(tree, z->parent->parent);
            }
        }
        
        // Дополнительная проверка для избежания бесконечного цикла
        if (z == tree->root) {
            break;
        }
    }
    tree->root->color = Color::BLACK;
}

void addRBNode(RBTree* tree, int value){
    RBNode* new_RBNode = new RBNode{value, Color::RED, nullptr, nullptr, nullptr};
    if (tree -> root == nullptr){ // если дерево пустое
        new_RBNode -> color = Color::BLACK;
        tree -> root = new_RBNode;
        return;
    }
    RBNode* address = tree -> root;
    while(1){
        if(value < address -> data){ //левое поддерево
            if (address -> left == nullptr){
                new_RBNode -> parent = address;
                address -> left = new_RBNode;
                fixAdd(tree, new_RBNode);
                return;
            }else{
                address = address -> left;
            }
        }
        else{ //правое поддерево
            if (address -> right == nullptr){
                new_RBNode -> parent = address;
                address -> right = new_RBNode;
                fixAdd(tree, new_RBNode);
                return;
            }else{
                address = address -> right;
            }
        }
    }  
}

RBNode* getRBNode(RBTree* tree, int value) {
    if (tree->root == nullptr) {
        cout << "Дерево пустое" << endl;
        return nullptr;
    }
    
    RBNode* address = tree->root;
    while (address != nullptr) {
        if (value == address->data) {
            return address;
        } else if (value < address->data) {
            address = address->left;
        } else {
            address = address->right;
        }
    }
    
    cout << "Узел " << value << " отсутствует" << endl;
    return nullptr;
}

RBNode* treeMinimum(RBTree* tree, RBNode* node) {
    if (node == nullptr) return nullptr;
    RBNode* address = node;
    while (address->left != nullptr) {
        address = address->left;
    }
    return address;
}

void deleteFix(RBTree* tree, RBNode* x) {
    while (x != tree->root && (x == nullptr || x->color == Color::BLACK)) {
        if (x == nullptr) break;
        // y - узел, который фактически удаляется
        // x - узел, который занимает место y
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w == nullptr) break;
            
            // Случай 1: брат красный
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
                if (w == nullptr) break;
            }
            
            // Случай 2: оба ребенка брата черные
            bool leftBlack = (w->left == nullptr) || (w->left->color == Color::BLACK);
            bool rightBlack = (w->right == nullptr) || (w->right->color == Color::BLACK);
            
            if (leftBlack && rightBlack) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                // Случай 3: правый ребенок брата черный, левый красный
                if (w->right == nullptr || w->right->color == Color::BLACK) {
                    if (w->left != nullptr) {
                        w->left->color = Color::BLACK;
                    }
                    w->color = Color::RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                    if (w == nullptr) break;
                }
                
                // Случай 4: правый ребенок брата красный
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                if (w->right != nullptr) {
                    w->right->color = Color::BLACK;
                }
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w == nullptr) break;
            
            // Случай 1: брат красный
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
                if (w == nullptr) break;
            }
            
            // Случай 2: оба ребенка брата черные
            bool leftBlack = (w->left == nullptr) || (w->left->color == Color::BLACK);
            bool rightBlack = (w->right == nullptr) || (w->right->color == Color::BLACK);
            
            if (leftBlack && rightBlack) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                // Случай 3: левый ребенок брата черный, правый красный
                if (w->left == nullptr || w->left->color == Color::BLACK) {
                    if (w->right != nullptr) {
                        w->right->color = Color::BLACK;
                    }
                    w->color = Color::RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                    if (w == nullptr) break;
                }
                
                // Случай 4: левый ребенок брата красный
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                if (w->left != nullptr) {
                    w->left->color = Color::BLACK;
                }
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    
    if (x != nullptr) {
        x->color = Color::BLACK;
    }
}

void transplant(RBTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == nullptr) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

void delRBNode(RBTree* tree, int value) {
    RBNode* z = getRBNode(tree, value);
    if (z == nullptr) return;
    
    RBNode* y = z;
    RBNode* x = nullptr;
    Color y_original_color = y->color;
    
    // Всегда гарантируем, что x не будет nullptr
    if (z->left == nullptr) {
        x = z->right;
        if (x == nullptr) {
            // Создаем временный фиктивный узел
            x = new RBNode{-1, Color::BLACK, z->parent, nullptr, nullptr};
        }
        transplant(tree, z, x);
    } else if (z->right == nullptr) {
        x = z->left;
        if (x == nullptr) {
            x = new RBNode{-1, Color::BLACK, z->parent, nullptr, nullptr};
        }
        transplant(tree, z, x);
    } else {
        y = treeMinimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        
        if (x == nullptr) {
            x = new RBNode{-1, Color::BLACK, y, nullptr, nullptr};
        }
        
        if (y->parent != z) {
            transplant(tree, y, x);
            y->right = z->right;
            y->right->parent = y;
        } else {
            x->parent = y;
        }
        
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    delete z;
    
    if (y_original_color == Color::BLACK) {
        deleteFix(tree, x);
    }
    
    // Удаляем фиктивный узел если он был создан
    if (x->data == -1) {
        if (x->parent != nullptr) {
            if (x->parent->left == x) x->parent->left = nullptr;
            else if (x->parent->right == x) x->parent->right = nullptr;
        }
        delete x;
    }
}

void printTree(RBNode* RBNode, int space = 0, int gap = 5) {
    if (RBNode == nullptr) {
        return;
    }
    
    // Сначала правые поддеревья
    printTree(RBNode->right, space + gap, gap);
    
    // Текущий узел
    for (int i = 0; i < space; i++) {
        cout << " ";
    }
    cout << RBNode->data << "(";
    if (RBNode->color == Color::RED) {
        cout << "R";
    } else {
        cout << "B";
    }
    cout << ")" << endl;
    
    // Затем левые поддеревья
    printTree(RBNode->left, space + gap, gap);
}

void print(RBTree* tree) {
    if (tree->root == nullptr) {
        cout << "Дерево пустое" << endl;
        return;
    }
    
    cout << "Структура дерева:" << endl;
    printTree(tree->root);
}