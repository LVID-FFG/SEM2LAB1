// g++ lab1_test.cpp Array.cpp FList.cpp LList.cpp Queue.cpp Steck.cpp Tree.cpp -o lab1
#include <iostream>

#include "Array.h"
#include "FList.h"
#include "LList.h"
#include "Queue.h"
#include "Steck.h"
#include "Tree.h"




#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// ==================== СИСТЕМА УПРАВЛЕНИЯ БАЗОЙ ДАННЫХ ====================

class Database {
private:
    map<string, Array> arrays;
    map<string, FList> flists;
    map<string, LList> llists;
    map<string, Queue> queues;
    map<string, Stack> stacks;
    map<string, RBTree*> trees;
    
    string filename;

public:
    Database(const string& file) : filename(file) {}

    // ==================== СЕРИАЛИЗАЦИЯ ====================

void saveToFile() {
    ofstream file(filename, ios::binary);
    if (!file) {
        cout << "Ошибка, невозможно открыть файл" << filename << endl;
        return;
    }

    // Сохраняем массивы
    size_t arrayCount = arrays.size();
    file.write(reinterpret_cast<char*>(&arrayCount), sizeof(size_t));
    for (const auto& pair : arrays) {
        // Сохраняем имя
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Сохраняем данные массива
        size_t size = pair.second.end - pair.second.data;
        file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        if (size > 0) {
            file.write(reinterpret_cast<char*>(pair.second.data), size * sizeof(int));
        }
    }

    // Сохраняем односвязные списки
    size_t flistCount = flists.size();
    file.write(reinterpret_cast<char*>(&flistCount), sizeof(size_t));
    for (const auto& pair : flists) {
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Подсчитываем элементы
        int count = 0;
        FLNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file.write(reinterpret_cast<char*>(&count), sizeof(int));
        current = pair.second.head;
        while (current != nullptr) {
            file.write(reinterpret_cast<char*>(&(current->data)), sizeof(int));
            current = current->next;
        }
    }

    // Сохраняем двусвязные списки
    size_t llistCount = llists.size();
    file.write(reinterpret_cast<char*>(&llistCount), sizeof(size_t));
    for (const auto& pair : llists) {
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Подсчитываем элементы
        int count = 0;
        LLNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file.write(reinterpret_cast<char*>(&count), sizeof(int));
        current = pair.second.head;
        while (current != nullptr) {
            file.write(reinterpret_cast<char*>(&(current->data)), sizeof(int));
            current = current->next;
        }
    }

    // Сохраняем стеки
    size_t stackCount = stacks.size();
    file.write(reinterpret_cast<char*>(&stackCount), sizeof(size_t));
    for (const auto& pair : stacks) {
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Подсчитываем элементы
        int count = 0;
        SNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        // Сохраняем в обратном порядке для стека
        file.write(reinterpret_cast<char*>(&count), sizeof(int));
        vector<int> temp;
        current = pair.second.head;
        while (current != nullptr) {
            temp.push_back(current->data);
            current = current->next;
        }
        // Записываем от дна к вершине
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            file.write(reinterpret_cast<char*>(&(*it)), sizeof(int));
        }
    }

    // Сохраняем очереди
    size_t queueCount = queues.size();
    file.write(reinterpret_cast<char*>(&queueCount), sizeof(size_t));
    for (const auto& pair : queues) {
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Подсчитываем элементы
        int count = 0;
        QNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file.write(reinterpret_cast<char*>(&count), sizeof(int));
        current = pair.second.head;
        while (current != nullptr) {
            file.write(reinterpret_cast<char*>(&(current->data)), sizeof(int));
            current = current->next;
        }
    }

    // Сохраняем деревья
    size_t treeCount = trees.size();
    file.write(reinterpret_cast<char*>(&treeCount), sizeof(size_t));
    for (const auto& pair : trees) {
        size_t nameLength = pair.first.length();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        file.write(pair.first.c_str(), nameLength);
        
        // Сохраняем дерево в порядке предварительного обхода
        saveTreeToFile(file, pair.second->root);
    }

    file.close();
    cout << "База данных сохранена " << filename << endl;
}

// Вспомогательная функция для сохранения дерева
void saveTreeToFile(ofstream& file, RBNode* node) {
    if (node == nullptr) {
        int marker = -1; // Маркер пустого узла
        file.write(reinterpret_cast<char*>(&marker), sizeof(int));
        return;
    }
    
    // Сохраняем данные узла и цвет
    file.write(reinterpret_cast<char*>(&(node->data)), sizeof(int));
    int color = (node->color == Color::RED) ? 0 : 1;
    file.write(reinterpret_cast<char*>(&color), sizeof(int));
    
    // Рекурсивно сохраняем левое и правое поддеревья
    saveTreeToFile(file, node->left);
    saveTreeToFile(file, node->right);
}

void loadFromFile() {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Файл " << filename << " ненайден, создан новый файл" << endl;
        return;
    }

    // Загружаем массивы
    size_t arrayCount;
    file.read(reinterpret_cast<char*>(&arrayCount), sizeof(size_t));
    for (size_t i = 0; i < arrayCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        
        Array arr = MCREATE();
        if (size > 0) {
            delete[] arr.data;
            arr.data = new int[size];
            arr.end = arr.data + size;
            file.read(reinterpret_cast<char*>(arr.data), size * sizeof(int));
        }
        arrays[name] = arr;
    }

    // Загружаем односвязные списки
    size_t flistCount;
    file.read(reinterpret_cast<char*>(&flistCount), sizeof(size_t));
    for (size_t i = 0; i < flistCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        FList flist;
        if (count > 0) {
            int data;
            file.read(reinterpret_cast<char*>(&data), sizeof(int));
            FCREATE(flist, data);
            
            for (int j = 1; j < count; j++) {
                file.read(reinterpret_cast<char*>(&data), sizeof(int));
                FADDTAIL(flist, data);
            }
        }
        flists[name] = flist;
    }

    // Загружаем двусвязные списки
    size_t llistCount;
    file.read(reinterpret_cast<char*>(&llistCount), sizeof(size_t));
    for (size_t i = 0; i < llistCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        LList llist;
        if (count > 0) {
            int data;
            file.read(reinterpret_cast<char*>(&data), sizeof(int));
            LCREATE(llist, data);
            
            for (int j = 1; j < count; j++) {
                file.read(reinterpret_cast<char*>(&data), sizeof(int));
                LADDTAIL(llist, data);
            }
        }
        llists[name] = llist;
    }

    // Загружаем стеки
    size_t stackCount;
    file.read(reinterpret_cast<char*>(&stackCount), sizeof(size_t));
    for (size_t i = 0; i < stackCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        Stack stack;
        if (count > 0) {
            vector<int> temp(count);
            for (int j = 0; j < count; j++) {
                file.read(reinterpret_cast<char*>(&temp[j]), sizeof(int));
            }
            // Восстанавливаем стек (первый элемент в файле - дно стека)
            SCREATE(stack, temp[0]);
            for (int j = 1; j < count; j++) {
                SPUSH(stack, temp[j]);
            }
        }
        stacks[name] = stack;
    }

    // Загружаем очереди
    size_t queueCount;
    file.read(reinterpret_cast<char*>(&queueCount), sizeof(size_t));
    for (size_t i = 0; i < queueCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        int count;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        Queue queue;
        if (count > 0) {
            int data;
            file.read(reinterpret_cast<char*>(&data), sizeof(int));
            QCREATE(queue, data);
            
            for (int j = 1; j < count; j++) {
                file.read(reinterpret_cast<char*>(&data), sizeof(int));
                QPUSH(queue, data);
            }
        }
        queues[name] = queue;
    }

    // Загружаем деревья
    size_t treeCount;
    file.read(reinterpret_cast<char*>(&treeCount), sizeof(size_t));
    for (size_t i = 0; i < treeCount; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
        string name(nameLength, ' ');
        file.read(&name[0], nameLength);
        
        RBTree* tree = new RBTree{nullptr};
        tree->root = loadTreeFromFile(file);
        trees[name] = tree;
    }

    file.close();
    cout << "База данных загружена из " << filename << endl;
}

// Вспомогательная функция для загрузки дерева
RBNode* loadTreeFromFile(ifstream& file) {
    int data;
    file.read(reinterpret_cast<char*>(&data), sizeof(int));
    
    if (data == -1) { // Маркер пустого узла
        return nullptr;
    }
    
    int color;
    file.read(reinterpret_cast<char*>(&color), sizeof(int));
    
    RBNode* node = new RBNode{data, (color == 0) ? Color::RED : Color::BLACK, 
                             nullptr, nullptr, nullptr};
    
    node->left = loadTreeFromFile(file);
    if (node->left != nullptr) {
        node->left->parent = node;
    }
    
    node->right = loadTreeFromFile(file);
    if (node->right != nullptr) {
        node->right->parent = node;
    }
    
    return node;
}

    // ==================== ОБРАБОТКА ЗАПРОСОВ ====================

string executeQuery(const string& query) {
        vector<string> tokens;
        stringstream ss(query);
        string token;
        
        while (ss >> token) {
            tokens.push_back(token);
        }
        
        if (tokens.empty()) return "Ошибка, запрос пуст";
        
        string command = tokens[0];
        transform(command.begin(), command.end(), command.begin(), ::toupper);
        
        // ==================== МАССИВЫ ====================
        if (command == "MCREATE" && tokens.size() == 2) {
            string name = tokens[1];
            arrays[name] = MCREATE();
            return "Запрос успешно выполнен";
        }
        else if (command == "MSIZE" && tokens.size() == 2) {
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                return "Массив ненайден";
            }
            size_t size = arrays[name].end - arrays[name].data;
            return to_string(size);
        }
        else if (command == "MPUSHEND" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (arrays.find(name) == arrays.end()) {
                arrays[name] = MCREATE();
            }
            MPUSHEND(arrays[name], value);
            return to_string(value);
        }
        else if (command == "MPUSH" && tokens.size() == 4) {
            string name = tokens[1];
            int index = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (arrays.find(name) == arrays.end()) {
                arrays[name] = MCREATE();
            }
            MPUSH(arrays[name], value, index);
            return to_string(value);
        }
        else if (command == "MDEL" && tokens.size() == 3) {
            string name = tokens[1];
            int index = stoi(tokens[2]);
            
            if (arrays.find(name) == arrays.end()) {
                return "Массив не найден";
            }
            MDEL(arrays[name], index);
            return "Запрос успешно выполнен";
        }
        else if (command == "MSWAP" && tokens.size() == 4) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            int index = stoi(tokens[3]);
            
            if (arrays.find(name) == arrays.end()) {
                return "Массив не найден";
            }
            MSWAP(arrays[name], value, index);
            return "Запрос успешно выполнен";
        }
        else if (command == "MGET" && tokens.size() == 3) {
            string name = tokens[1];
            int index = stoi(tokens[2]);
            
            if (arrays.find(name) == arrays.end()) {
                return "Массив не найден";
            }
            int value = MGET(arrays[name], index);
            return to_string(value);
        }
        else if (command == "MPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (arrays.find(name) == arrays.end()) {
                return "Массив не найден";
            }
            MPRINT(arrays[name]);
            return "Запрос успешно выполнен";
        }
        
        // ==================== ОДНОСВЯЗНЫЕ СПИСКИ ====================
        else if (command == "FCREATE" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            FCREATE(flists[name], value);
            return to_string(value);
        }
        else if (command == "FGETINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int index = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* node = FGETINDEX(flists[name], index);
            return to_string(node->data);
        }
        else if (command == "FGETDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* node = FGETDATA(flists[name], value);
            return to_string(node->data);
        }
        else if (command == "FADDTAIL" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FADDTAIL(flists[name], value);
            return to_string(value);
        }
        else if (command == "FADDHEAD" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FADDHEAD(flists[name], value);
            return to_string(value);
        }
        // Добавление ПОСЛЕ по ИНДЕКСУ
        else if (command == "FADDAFTERINDEX" && tokens.size() == 4) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
            FADDAFTER(targetNode, value);
            return to_string(value);
        }
        // Добавление ПОСЛЕ по ЗНАЧЕНИЮ
        else if (command == "FADDAFTERDATA" && tokens.size() == 4) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETDATA(flists[name], targetValue);
            FADDAFTER(targetNode, value);
            return to_string(value);
        }
        // Добавление ДО по ИНДЕКСУ
        else if (command == "FADDBEFOREINDEX" && tokens.size() == 4) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
            FADDBEFORE(flists[name], targetNode, value);
            return to_string(value);
        }
        // Добавление ДО по ЗНАЧЕНИЮ
        else if (command == "FADDBEFOREDATA" && tokens.size() == 4) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETDATA(flists[name], targetValue);
            FADDBEFORE(flists[name], targetNode, value);
            return to_string(value);
        }
        else if (command == "FDELTAIL" && tokens.size() == 2) {
            string name = tokens[1];
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FDELTAIL(flists[name]);
            return "Запрос успешно выполнен";
        }
        else if (command == "FDELHEAD" && tokens.size() == 2) {
            string name = tokens[1];
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FDELHEAD(flists[name]);
            return "Запрос успешно выполнен";
        }
        // Удаление ПОСЛЕ по ИНДЕКСУ
        else if (command == "FDELAFTERINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
            FDELAFTER(targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ПОСЛЕ по ЗНАЧЕНИЮ
        else if (command == "FDELAFTERDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETDATA(flists[name], targetValue);
            FDELAFTER(targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ДО по ИНДЕКСУ
        else if (command == "FDELBEFOREINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
            FDELBEFORE(flists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ДО по ЗНАЧЕНИЮ
        else if (command == "FDELBEFOREDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FLNode* targetNode = FGETDATA(flists[name], targetValue);
            FDELBEFORE(flists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        else if (command == "FDELDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FDELDATA(flists[name], value);
            return "Запрос успешно выполнен";
        }
        else if (command == "FPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (flists.find(name) == flists.end()) {
                return "Список не найден";
            }
            FPRINT(flists[name]);
            return "Запрос успешно выполнен";
        }
        
        // ==================== ДВУСВЯЗНЫЕ СПИСКИ ====================
        else if (command == "LCREATE" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            LCREATE(llists[name], value);
            return to_string(value);
        }
        else if (command == "LGETINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int index = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* node = LGETINDEX(llists[name], index);
            return to_string(node->data);
        }
        else if (command == "LGETDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* node = LGETDATA(llists[name], value);
            return to_string(node->data);
        }
        else if (command == "LADDTAIL" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LADDTAIL(llists[name], value);
            return to_string(value);
        }
        else if (command == "LADDHEAD" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LADDHEAD(llists[name], value);
            return to_string(value);
        }
        // Добавление ПОСЛЕ по ИНДЕКСУ
        else if (command == "LADDAFTERINDEX" && tokens.size() == 4) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
            LADDAFTER(llists[name], targetNode, value);
            return to_string(value);
        }
        // Добавление ПОСЛЕ по ЗНАЧЕНИЮ
        else if (command == "LADDAFTERDATA" && tokens.size() == 4) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETDATA(llists[name], targetValue);
            LADDAFTER(llists[name], targetNode, value);
            return to_string(value);
        }
        // Добавление ДО по ИНДЕКСУ
        else if (command == "LADDBEFOREINDEX" && tokens.size() == 4) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
            LADDBEFORE(llists[name], targetNode, value);
            return to_string(value);
        }
        // Добавление ДО по ЗНАЧЕНИЮ
        else if (command == "LADDBEFOREDATA" && tokens.size() == 4) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            int value = stoi(tokens[3]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETDATA(llists[name], targetValue);
            LADDBEFORE(llists[name], targetNode, value);
            return to_string(value);
        }
        else if (command == "LDELTAIL" && tokens.size() == 2) {
            string name = tokens[1];
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LDELTAIL(llists[name]);
            return "Запрос успешно выполнен";
        }
        else if (command == "LDELHEAD" && tokens.size() == 2) {
            string name = tokens[1];
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LDELHEAD(llists[name]);
            return "Запрос успешно выполнен";
        }
        // Удаление ПОСЛЕ по ИНДЕКСУ
        else if (command == "LDELAFTERINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
            LDELAFTER(llists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ПОСЛЕ по ЗНАЧЕНИЮ
        else if (command == "LDELAFTERDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETDATA(llists[name], targetValue);
            LDELAFTER(llists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ДО по ИНДЕКСУ
        else if (command == "LDELBEFOREINDEX" && tokens.size() == 3) {
            string name = tokens[1];
            int targetIndex = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
            LDELBEFORE(llists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        // Удаление ДО по ЗНАЧЕНИЮ
        else if (command == "LDELBEFOREDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int targetValue = stoi(tokens[2]);
            
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LLNode* targetNode = LGETDATA(llists[name], targetValue);
            LDELBEFORE(llists[name], targetNode);
            return "Запрос успешно выполнен";
        }
        else if (command == "LDELDATA" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LDELDATA(llists[name], value);
            return "Запрос успешно выполнен";
        }
        else if (command == "LPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (llists.find(name) == llists.end()) {
                return "Список не найден";
            }
            LPRINT(llists[name]);
            return "Запрос успешно выполнен";
        }
        
        // ==================== ОЧЕРЕДИ ====================
        else if (command == "QCREATE" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            QCREATE(queues[name], value);
            return to_string(value);
        }
        else if (command == "QPUSH" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (queues.find(name) == queues.end()) {
                return "Очередь не найдена";
            }
            QPUSH(queues[name], value);
            return to_string(value);
        }
        else if (command == "QPOP" && tokens.size() == 2) {
            string name = tokens[1];
            if (queues.find(name) == queues.end() || queues[name].head == nullptr) {
                return "Очередь не существует или пуста";
            }
            int value = queues[name].head->data;
            QPOP(queues[name]);
            return to_string(value);
        }
        else if (command == "QPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (queues.find(name) == queues.end()) {
                return "Очередь не найдена";
            }
            QPRINT(queues[name]);
            return "Запрос успешно выполнен";
        }
        
        // ==================== СТЕКИ ====================
        // Создание стека (только если не существует)
        else if (command == "SCREATE" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (stacks.find(name) != stacks.end()) {
                return "Стэк с таким именем уже существует";
            }
            SCREATE(stacks[name], value);
            return to_string(value);
        }
        // Добавление в стек
        else if (command == "SPUSH" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (stacks.find(name) == stacks.end()) {
                return "Стэк не найден";
            }
            SPUSH(stacks[name], value);
            return to_string(value);
        }
        else if (command == "SPOP" && tokens.size() == 2) {
            string name = tokens[1];
            if (stacks.find(name) == stacks.end() || stacks[name].head == nullptr) {
                return "Стэк пуст или не существует";
            }
            int value = stacks[name].head->data;
            SPOP(stacks[name]);
            return to_string(value);
        }
        else if (command == "SPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (stacks.find(name) == stacks.end()) {
                return "Стэк не найден";
            }
            SPRINT(stacks[name]);
            return "Запрос успешно выполнен";
        }
        
        // ==================== ДЕРЕВЬЯ ====================
        else if (command == "TADD" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (trees.find(name) == trees.end()) {
                trees[name] = new RBTree{nullptr};
            }
            addRBNode(trees[name], value);
            return to_string(value);
        }
        else if (command == "TDEL" && tokens.size() == 3) {
            string name = tokens[1];
            int value = stoi(tokens[2]);
            
            if (trees.find(name) == trees.end()) {
                return "Дерево не найдено";
            }
            delRBNode(trees[name], value);
            return to_string(value);
        }
        else if (command == "TPRINT" && tokens.size() == 2) {
            string name = tokens[1];
            if (trees.find(name) == trees.end()) {
                return "Дерево не найдено";
            }
            print(trees[name]);
            return "Запрос успешно выполнен";
        }
        
        else {
            return "Ошибка, неизвестная комманда или неправильные аргументы";
        }
    }
};

// ==================== ОБРАБОТКА АРГУМЕНТОВ КОМАНДНОЙ СТРОКИ ====================

void printUsage() {
    cout << "Использование: ./lab1 --file <имя_файла> --query '<команда>'" << endl;
    cout << endl;
    cout << "МАССИВЫ:" << endl;
    cout << "  MCREATE <имя>                          - Создать массив" << endl;
    cout << "  MSIZE <имя>                            - Получить размер массива" << endl;
    cout << "  MPUSHEND <имя> <значение>              - Добавить в конец массива" << endl;
    cout << "  MPUSH <имя> <индекс> <значение>        - Добавить по индексу" << endl;
    cout << "  MDEL <имя> <индекс>                    - Удалить по индексу" << endl;
    cout << "  MSWAP <имя> <значение> <индекс>        - Заменить элемент по индексу" << endl;
    cout << "  MGET <имя> <индекс>                    - Получить элемент по индексу" << endl;
    cout << "  MPRINT <имя>                           - Вывести массив" << endl;
    cout << endl;
    
    cout << "ОДНОСВЯЗНЫЕ СПИСКИ:" << endl;
    cout << "  FCREATE <имя> <значение>               - Создать список" << endl;
    cout << "  FGETINDEX <имя> <индекс>               - Получить значение по индексу" << endl;
    cout << "  FGETDATA <имя> <значение>              - Найти узел по значению" << endl;
    cout << "  FADDTAIL <имя> <значение>              - Добавить в конец" << endl;
    cout << "  FADDHEAD <имя> <значение>              - Добавить в начало" << endl;
    cout << "  FADDAFTERINDEX <имя> <индекс> <знач>   - Добавить после индекса" << endl;
    cout << "  FADDAFTERDATA <имя> <знач> <нов_знач>  - Добавить после значения" << endl;
    cout << "  FADDBEFOREINDEX <имя> <индекс> <знач>  - Добавить до индекса" << endl;
    cout << "  FADDBEFOREDATA <имя> <знач> <нов_знач> - Добавить до значения" << endl;
    cout << "  FDELTAIL <имя>                         - Удалить хвост" << endl;
    cout << "  FDELHEAD <имя>                         - Удалить голову" << endl;
    cout << "  FDELAFTERINDEX <имя> <индекс>          - Удалить после индекса" << endl;
    cout << "  FDELAFTERDATA <имя> <значение>         - Удалить после значения" << endl;
    cout << "  FDELBEFOREINDEX <имя> <индекс>         - Удалить до индекса" << endl;
    cout << "  FDELBEFOREDATA <имя> <значение>        - Удалить до значения" << endl;
    cout << "  FDELDATA <имя> <значение>              - Удалить по значению" << endl;
    cout << "  FPRINT <имя>                           - Вывести список" << endl;
    cout << endl;
    
    cout << "ДВУСВЯЗНЫЕ СПИСКИ:" << endl;
    cout << "  LCREATE <имя> <значение>               - Создать список" << endl;
    cout << "  LGETINDEX <имя> <индекс>               - Получить значение по индексу" << endl;
    cout << "  LGETDATA <имя> <значение>              - Найти узел по значению" << endl;
    cout << "  LADDTAIL <имя> <значение>              - Добавить в конец" << endl;
    cout << "  LADDHEAD <имя> <значение>              - Добавить в начало" << endl;
    cout << "  LADDAFTERINDEX <имя> <индекс> <знач>   - Добавить после индекса" << endl;
    cout << "  LADDAFTERDATA <имя> <знач> <нов_знач>  - Добавить после значения" << endl;
    cout << "  LADDBEFOREINDEX <имя> <индекс> <знач>  - Добавить до индекса" << endl;
    cout << "  LADDBEFOREDATA <имя> <знач> <нов_знач> - Добавить до значения" << endl;
    cout << "  LDELTAIL <имя>                         - Удалить хвост" << endl;
    cout << "  LDELHEAD <имя>                         - Удалить голову" << endl;
    cout << "  LDELAFTERINDEX <имя> <индекс>          - Удалить после индекса" << endl;
    cout << "  LDELAFTERDATA <имя> <значение>         - Удалить после значения" << endl;
    cout << "  LDELBEFOREINDEX <имя> <индекс>         - Удалить до индекса" << endl;
    cout << "  LDELBEFOREDATA <имя> <значение>        - Удалить до значения" << endl;
    cout << "  LDELDATA <имя> <значение>              - Удалить по значению" << endl;
    cout << "  LPRINT <имя>                           - Вывести список" << endl;
    cout << endl;
    
    cout << "ОЧЕРЕДИ:" << endl;
    cout << "  QCREATE <имя> <значение>               - Создать очередь" << endl;
    cout << "  QPUSH <имя> <значение>                 - Добавить в очередь" << endl;
    cout << "  QPOP <имя>                             - Извлечь из очереди" << endl;
    cout << "  QPRINT <имя>                           - Вывести очередь" << endl;
    cout << endl;
    
    cout << "СТЕКИ:" << endl;
    cout << "  SCREATE <имя> <значение>               - Создать стек" << endl;
    cout << "  SPUSH <имя> <значение>                 - Добавить в стек" << endl;
    cout << "  SPOP <имя>                             - Извлечь из стека" << endl;
    cout << "  SPRINT <имя>                           - Вывести стек" << endl;
    cout << endl;
    
    cout << "КРАСНО-ЧЕРНЫЕ ДЕРЕВЬЯ:" << endl;
    cout << "  TADD <имя> <значение>                  - Добавить в дерево" << endl;
    cout << "  TDEL <имя> <значение>                  - Удалить из дерева" << endl;
    cout << "  TPRINT <имя>                           - Вывести дерево" << endl;
    cout << endl;
}

int main(int argc, char* argv[]) {
    string filename;
    string query;
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--help") {
            printUsage();
            return 0;
        }
    }
    
    if (filename.empty() || query.empty()) {
        printUsage();
        return 1;
    }
    
    // Создаем и загружаем базу данных
    Database db(filename);
    db.loadFromFile();
    
    // Выполняем запрос
    string result = db.executeQuery(query);
    cout << result << endl;
    
    // Сохраняем изменения
    db.saveToFile();
    
    return 0;
}