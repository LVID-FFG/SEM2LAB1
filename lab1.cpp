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
    ~Database() {
        // Освобождаем память деревьев
        for (auto& pair : trees) {
            deleteTree(pair.second->root);
            delete pair.second;
        }
    }
    void deleteTree(RBNode* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    // ==================== СЕРИАЛИЗАЦИЯ ====================

void saveToFile() {
    ofstream file(filename);
    if (!file) {
        cout << "Ошибка, невозможно открыть файл " << filename << endl;
        return;
    }

    // Сохраняем массивы
    file << "ARRAYS " << arrays.size() << endl;
    for (const auto& pair : arrays) {
        file << "ARRAY " << pair.first << endl;
        
        size_t size = pair.second.size;
        file << size << endl;
        
        // Сохраняем элементы только если массив не пустой
        if (size > 0) {
            for (size_t i = 0; i < size; i++) {
                file << pair.second.data[i] << endl;
            }
        }
    }

    // Сохраняем односвязные списки
    file << "FLISTS " << flists.size() << endl;
    for (const auto& pair : flists) {
        file << "FLIST " << pair.first << endl;
        
        // Подсчитываем элементы
        int count = 0;
        FLNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file << count << endl;
        current = pair.second.head;
        while (current != nullptr) {
            file << current->data << endl;
            current = current->next;
        }
    }

    // Сохраняем двусвязные списки
    file << "LLISTS " << llists.size() << endl;
    for (const auto& pair : llists) {
        file << "LLIST " << pair.first << endl;
        
        // Подсчитываем элементы
        int count = 0;
        LLNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file << count << endl;
        current = pair.second.head;
        while (current != nullptr) {
            file << current->data << endl;
            current = current->next;
        }
    }

    // Сохраняем стеки
    file << "STACKS " << stacks.size() << endl;
    for (const auto& pair : stacks) {
        file << "STACK " << pair.first << endl;
        
        // Подсчитываем элементы
        int count = 0;
        SNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        // Сохраняем в обратном порядке для стека
        file << count << endl;
        vector<string> temp;
        current = pair.second.head;
        while (current != nullptr) {
            temp.push_back(current->data);
            current = current->next;
        }
        // Записываем от дна к вершине
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            file << *it << endl;
        }
    }

    // Сохраняем очереди
    file << "QUEUES " << queues.size() << endl;
    for (const auto& pair : queues) {
        file << "QUEUE " << pair.first << endl;
        
        // Подсчитываем элементы
        int count = 0;
        QNode* current = pair.second.head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        
        file << count << endl;
        current = pair.second.head;
        while (current != nullptr) {
            file << current->data << endl;
            current = current->next;
        }
    }

    // Сохраняем деревья (используем бинарный формат для совместимости)
    file << "TREES " << trees.size() << endl;
    for (const auto& pair : trees) {
        file << "TREE " << pair.first << endl;
        
        // Сохраняем дерево в порядке предварительного обхода
        vector<string> treeData;
        saveTreeToString(pair.second->root, treeData);
        file << treeData.size() << endl;
        for (const auto& data : treeData) {
            file << data << endl;
        }
    }

    file.close();
    cout << "База данных сохранена " << filename << endl;
}

// Вспомогательная функция для сохранения дерева в строки
void saveTreeToString(RBNode* node, vector<string>& result) {
    if (node == nullptr) {
        result.push_back("NULL");
        return;
    }
    
    // Сохраняем данные узла (int) и цвет
    result.push_back(to_string(node->data));
    result.push_back((node->color == Color::RED) ? "RED" : "BLACK");
    
    // Рекурсивно сохраняем левое и правое поддеревья
    saveTreeToString(node->left, result);
    saveTreeToString(node->right, result);
}

void loadFromFile() {
    ifstream file(filename);
    if (!file) {
        cout << "Файл " << filename << " ненайден, создан новый файл" << endl;
        return;
    }

    string section;
    int count;
    
    while (file >> section) {
        if (section == "ARRAYS") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int size;
                file >> size;
                file.ignore();
                
                Array arr;
                arr.size = size;
                if (size > 0) {
                    // Выделяем память для массива
                    arr.data = new string[size];
                    for (int j = 0; j < size; j++) {
                        getline(file, arr.data[j]);
                    }
                }
                arrays[name] = arr;
            }
        }
        else if (section == "FLISTS") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int elementCount;
                file >> elementCount;
                file.ignore(); // Пропускаем перевод строки
                
                FList flist;
                if (elementCount > 0) {
                    // Создаем временный вектор для хранения данных
                    vector<string> elements;
                    for (int j = 0; j < elementCount; j++) {
                        string data;
                        getline(file, data);
                        elements.push_back(data);
                    }
                    if (!elements.empty()) {
                        flist.head = new FLNode(elements[0]);
                        FLNode* current = flist.head;
                        
                        for (size_t j = 1; j < elements.size(); j++) {
                            current->next = new FLNode(elements[j]);
                            current = current->next;
                        }
                    }
                }
                flists[name] = flist;
            }
        }
        else if (section == "LLISTS") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int elementCount;
                file >> elementCount;
                file.ignore(); // Пропускаем перевод строки
                
                LList llist;
                if (elementCount > 0) {
                    string data;
                    getline(file, data);
                    LCREATE(llist, data);
                    
                    for (int j = 1; j < elementCount; j++) {
                        getline(file, data);
                        LADDTAIL(llist, data);
                    }
                }
                llists[name] = llist;
            }
        }
        else if (section == "STACKS") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int elementCount;
                file >> elementCount;
                file.ignore(); // Пропускаем перевод строки
                
                Stack stack;
                if (elementCount > 0) {
                    vector<string> temp(elementCount);
                    for (int j = 0; j < elementCount; j++) {
                        getline(file, temp[j]);
                    }
                    // Восстанавливаем стек
                    SCREATE(stack, temp[0]);
                    for (int j = 1; j < elementCount; j++) {
                        SPUSH(stack, temp[j]);
                    }
                }
                stacks[name] = stack;
            }
        }
        else if (section == "QUEUES") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int elementCount;
                file >> elementCount;
                file.ignore(); // Пропускаем перевод строки
                
                Queue queue;
                if (elementCount > 0) {
                    string data;
                    getline(file, data);
                    QCREATE(queue, data);
                    
                    for (int j = 1; j < elementCount; j++) {
                        getline(file, data);
                        QPUSH(queue, data);
                    }
                }
                queues[name] = queue;
            }
        }
        else if (section == "TREES") {
            file >> count;
            for (int i = 0; i < count; i++) {
                string type, name;
                file >> type >> name;
                
                int elementCount;
                file >> elementCount;
                file.ignore(); // Пропускаем перевод строки
                
                RBTree* tree = new RBTree{nullptr};
                vector<string> treeData(elementCount);
                for (int j = 0; j < elementCount; j++) {
                    getline(file, treeData[j]);
                }
                tree->root = loadTreeFromString(treeData, 0).first;
                trees[name] = tree;
            }
        }
    }

    file.close();
    cout << "База данных загружена из " << filename << endl;
}

// Вспомогательная функция для загрузки дерева из строк
pair<RBNode*, int> loadTreeFromString(const vector<string>& data, int index) {
    if (index >= data.size() || data[index] == "NULL") {
        return {nullptr, index + 1};
    }
    
    int nodeData = stoi(data[index]);  // Конвертируем строку в int
    string colorStr = data[index + 1];
    
    RBNode* node = new RBNode{nodeData, (colorStr == "RED") ? Color::RED : Color::BLACK, 
                             nullptr, nullptr, nullptr};
    
    auto leftResult = loadTreeFromString(data, index + 2);
    node->left = leftResult.first;
    if (node->left != nullptr) {
        node->left->parent = node;
    }
    
    auto rightResult = loadTreeFromString(data, leftResult.second);
    node->right = rightResult.first;
    if (node->right != nullptr) {
        node->right->parent = node;
    }
    
    return {node, rightResult.second};
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
    arrays[name] = Array();
    return "Запрос успешно выполнен";
    }
    else if (command == "MSIZE" && tokens.size() == 2) {
        string name = tokens[1];
        if (arrays.find(name) == arrays.end()) {
            return "Массив ненайден";
        }
        size_t size = arrays[name].size;
        return to_string(size);
    }
    else if (command == "MPUSHEND" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
    
        if (arrays.find(name) == arrays.end()) {
            arrays[name] = Array();
        }
        MPUSHEND(arrays[name], value);
        return value;
    }
    else if (command == "MPUSH" && tokens.size() == 4) {
        string name = tokens[1];
        int index = stoi(tokens[2]);
        string value = tokens[3];
        
        if (arrays.find(name) == arrays.end()) {
            arrays[name] = Array();
        }
        MPUSH(arrays[name], value, index);
        return value;
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
        string value = tokens[2];
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
        string value = MGET(arrays[name], index);
        return value;
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
        string value = tokens[2];
        FCREATE(flists[name], value);
        return value;
    }
    else if (command == "FGETINDEX" && tokens.size() == 3) {
        string name = tokens[1];
        int index = stoi(tokens[2]);
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* node = FGETINDEX(flists[name], index);
        return node->data;
    }
    else if (command == "FGETDATA" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* node = FGETDATA(flists[name], value);
        return node->data;
    }
    else if (command == "FADDTAIL" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FADDTAIL(flists[name], value);
        return value;
    }
    else if (command == "FADDHEAD" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FADDHEAD(flists[name], value);
        return value;
    }
    // Добавление ПОСЛЕ по ИНДЕКСУ
    else if (command == "FADDAFTERINDEX" && tokens.size() == 4) {
        string name = tokens[1];
        int targetIndex = stoi(tokens[2]);
        string value = tokens[3];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
        FADDAFTER(targetNode, value);
        return value;
    }
    // Добавление ПОСЛЕ по ЗНАЧЕНИЮ
    else if (command == "FADDAFTERDATA" && tokens.size() == 4) {
        string name = tokens[1];
        string targetValue = tokens[2];
        string value = tokens[3];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* targetNode = FGETDATA(flists[name], targetValue);
        FADDAFTER(targetNode, value);
        return value;
    }
    // Добавление ДО по ИНДЕКСУ
    else if (command == "FADDBEFOREINDEX" && tokens.size() == 4) {
        string name = tokens[1];
        int targetIndex = stoi(tokens[2]);
        string value = tokens[3];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* targetNode = FGETINDEX(flists[name], targetIndex);
        FADDBEFORE(flists[name], targetNode, value);
        return value;
    }
    // Добавление ДО по ЗНАЧЕНИЮ
    else if (command == "FADDBEFOREDATA" && tokens.size() == 4) {
        string name = tokens[1];
        string targetValue = tokens[2];
        string value = tokens[3];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* targetNode = FGETDATA(flists[name], targetValue);
        FADDBEFORE(flists[name], targetNode, value);
        return value;
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
        string targetValue = tokens[2];
        
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
        string targetValue = tokens[2];
        
        if (flists.find(name) == flists.end()) {
            return "Список не найден";
        }
        FLNode* targetNode = FGETDATA(flists[name], targetValue);
        FDELBEFORE(flists[name], targetNode);
        return "Запрос успешно выполнен";
    }
    else if (command == "FDELDATA" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
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
        string value = tokens[2];
        LCREATE(llists[name], value);
        return value;
    }
    else if (command == "LGETINDEX" && tokens.size() == 3) {
        string name = tokens[1];
        int index = stoi(tokens[2]);
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* node = LGETINDEX(llists[name], index);
        return node->data;
    }
    else if (command == "LGETDATA" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* node = LGETDATA(llists[name], value);
        return node->data;
    }
    else if (command == "LADDTAIL" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LADDTAIL(llists[name], value);
        return value;
    }
    else if (command == "LADDHEAD" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LADDHEAD(llists[name], value);
        return value;
    }
    // Добавление ПОСЛЕ по ИНДЕКСУ
    else if (command == "LADDAFTERINDEX" && tokens.size() == 4) {
        string name = tokens[1];
        int targetIndex = stoi(tokens[2]);
        string value = tokens[3];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
        LADDAFTER(llists[name], targetNode, value);
        return value;
    }
    // Добавление ПОСЛЕ по ЗНАЧЕНИЮ
    else if (command == "LADDAFTERDATA" && tokens.size() == 4) {
        string name = tokens[1];
        string targetValue = tokens[2];
        string value = tokens[3];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* targetNode = LGETDATA(llists[name], targetValue);
        LADDAFTER(llists[name], targetNode, value);
        return value;
    }
    // Добавление ДО по ИНДЕКСУ
    else if (command == "LADDBEFOREINDEX" && tokens.size() == 4) {
        string name = tokens[1];
        int targetIndex = stoi(tokens[2]);
        string value = tokens[3];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* targetNode = LGETINDEX(llists[name], targetIndex);
        LADDBEFORE(llists[name], targetNode, value);
        return value;
    }
    // Добавление ДО по ЗНАЧЕНИЮ
    else if (command == "LADDBEFOREDATA" && tokens.size() == 4) {
        string name = tokens[1];
        string targetValue = tokens[2];
        string value = tokens[3];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* targetNode = LGETDATA(llists[name], targetValue);
        LADDBEFORE(llists[name], targetNode, value);
        return value;
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
        string targetValue = tokens[2];
        
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
        string targetValue = tokens[2];
        
        if (llists.find(name) == llists.end()) {
            return "Список не найден";
        }
        LLNode* targetNode = LGETDATA(llists[name], targetValue);
        LDELBEFORE(llists[name], targetNode);
        return "Запрос успешно выполнен";
    }
    else if (command == "LDELDATA" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
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
        string value = tokens[2];
        QCREATE(queues[name], value);
        return value;
    }
    else if (command == "QPUSH" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (queues.find(name) == queues.end()) {
            return "Очередь не найдена";
        }
        QPUSH(queues[name], value);
        return value;
    }
    else if (command == "QPOP" && tokens.size() == 2) {
        string name = tokens[1];
        if (queues.find(name) == queues.end() || queues[name].head == nullptr) {
            return "Очередь не существует или пуста";
        }
        string value = queues[name].head->data;
        QPOP(queues[name]);
        return value;
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
    // Создание стека
    else if (command == "SCREATE" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (stacks.find(name) != stacks.end()) {
            return "Стэк с таким именем уже существует";
        }
        SCREATE(stacks[name], value);
        return value;
    }
    // Добавление в стек
    else if (command == "SPUSH" && tokens.size() == 3) {
        string name = tokens[1];
        string value = tokens[2];
        
        if (stacks.find(name) == stacks.end()) {
            return "Стэк не найден";
        }
        SPUSH(stacks[name], value);
        return value;
    }
    else if (command == "SPOP" && tokens.size() == 2) {
        string name = tokens[1];
        if (stacks.find(name) == stacks.end() || stacks[name].head == nullptr) {
            return "Стэк пуст или не существует";
        }
        string value = stacks[name].head->data;
        SPOP(stacks[name]);
        return value;
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