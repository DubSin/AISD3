#ifndef BLACKREDTREE_H
#define BLACKREDTREE_H

using namespace std;

#include "stdafx.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"

enum class Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
    
    Node(int value) : data(value), color(Color::RED), 
                   left(nullptr), right(nullptr), parent(nullptr) {}
};


class RedBlackTree {
private:
    Node* root;
    Node* nil; 
    int size;

public:
    RedBlackTree() {
        nil = new Node(0);
        nil->color = Color::BLACK;
        root = nil;
    }

    ~RedBlackTree() {
        clear();
        delete nil;
    }

    // Новый метод для чтения скобочной записи из файла
    bool parseAndBuildFromFile(const string& filename) {
        try {
            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Error: Cannot open file '" << filename << "'" << endl;
                return false;
            }
            
            string bracketNotation;
            getline(file, bracketNotation);
            file.close();
            
            if (bracketNotation.empty()) {
                cout << "Error: File is empty or contains no data" << endl;
                return false;
            }
            
            parseAndBuild(bracketNotation);
            return true;
        }
        catch (const exception& e) {
            cout << "Error reading file: " << e.what() << endl;
            return false;
        }
    }

    void parseAndBuild(const string& bracketNotation) {
        try{
            clear(); 
            
            if (bracketNotation.empty()) {
                return;
            }
            
            Stack<Node*> nodeStack;
            Stack<int> childCountStack; // Для отслеживания количества детей у каждого узла
            string currentNumber;
            bool readingNumber = false;
            Node* lastNode = nullptr; // Последний созданный узел
            
            for (size_t i = 0; i < bracketNotation.length(); i++) {
                char c = bracketNotation[i];
                
                if (isdigit(c)) {
                    currentNumber += c;
                    readingNumber = true;
                } else {
                    if (readingNumber) {
                        int value = stoi(currentNumber);
                        Node* newNode = new Node(value);
                        newNode->left = nil;
                        newNode->right = nil;
                        
                        if (root == nil) {
                            root = newNode;
                            newNode->parent = nil;
                            lastNode = newNode;
                        } else {
                            if (nodeStack.empty()) {
                                delete newNode;
                                clear();
                                throw invalid_argument("Incorrect bracket structure");
                            }
                            
                            Node* parent = nodeStack.top();
                            int& childCount = childCountStack.top();
                            if (childCount >= 2) {
                                delete newNode;
                                clear();
                                throw invalid_argument("Node cant have more than 2 kids");
                            }
                            
                            if (childCount == 0) {
                                parent->left = newNode;
                            } else {
                                parent->right = newNode;
                            }
                            newNode->parent = parent;
                            childCount++;
                            lastNode = newNode;
                        }
                        
                        currentNumber.clear();
                        readingNumber = false;
                    }
                    
                    if (c == '(') {
                        nodeStack.push(lastNode);
                        childCountStack.push(0);
                        lastNode = nullptr; 
                    } else if (c == ')') {
                        if (nodeStack.empty() || childCountStack.empty()) {
                            clear();
                            throw invalid_argument("Unbalanced brakets");
                        }
                        nodeStack.pop();
                        childCountStack.pop();
                    } else if (c == ',') {
                        continue;
                    } else if (!isspace(c)) {
                        clear();
                        throw invalid_argument("Unknow symbol in brakets");
                    }
                }
            }
            
            // Обрабатываем последнее число, если строка заканчивается числом
            if (readingNumber && !currentNumber.empty()) {
                int value = stoi(currentNumber);
                Node* newNode = new Node(value);
                newNode->left = nil;
                newNode->right = nil;
                
                if (root == nil) {
                    root = newNode;
                    newNode->parent = nil;
                } else {
                    if (nodeStack.empty()) {
                        delete newNode;
                        clear();
                        throw invalid_argument("Unbalanced brakets");
                    }
                    
                    Node* parent = nodeStack.top();
                    int& childCount = childCountStack.top();
                    if (childCount >= 2) {
                        delete newNode;
                        clear();
                        throw invalid_argument("node cant have more than two kids");
                    }
                    
                    if (childCount == 0) {
                        parent->left = newNode;
                    } else {
                        parent->right = newNode;
                    }
                    newNode->parent = parent;
                }
            }
            

            if (!nodeStack.empty() || !childCountStack.empty()) {
                clear();
                throw invalid_argument("Unbalanced brakets");
            }
            
            rebuildTree();
        }
        catch(const exception& e){
            cout << e.what() << endl;
            clear();
        }
    }

    void insert(int data) {
        Node* newNode = new Node(data);
        newNode->left = nil;
        newNode->right = nil;
        
        Node* y = nil;
        Node* x = root;
        
        while (x != nil) {
            y = x;
            if (newNode->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        
        newNode->parent = y;
        if (y == nil) {
            root = newNode;
        } else if (newNode->data < y->data) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }
        
        insertFixup(newNode);
    }

    Node* search(int data) {
        Node* node = searchHelper(root, data);
        return (node != nil) ? node : nullptr;
    }

    void remove(int data) {
        Node* node = search(data);
        if (node != nil) {
            deleteNode(node);
        }
    }

    void clear() {
        clearIterative();
    }

    void inOrderIterative() {
        if (root == nil) {
            cout << "Tree empty" << endl;
            return;
        }
        
        Stack<Node*> stack;
        Node* current = root;
        
        while (current != nil || !stack.empty()) {
            while (current != nil) {
                stack.push(current);
                current = current->left;
            }
            
            // Обрабатываем узел
            current = stack.top();
            stack.pop();
            cout << current->data << getColorSymbol(current->color) << " ";
            
            current = current->right;
        }
        cout << endl;
    }

    void preOrderIterative() {
        if (root == nil) {
            cout << "Tree empty" << endl;
            return;
        }
        
        Stack<Node*> stack;
        stack.push(root);
        
        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();
            cout << current->data << getColorSymbol(current->color) << " ";
            
            if (current->right != nil) {
                stack.push(current->right);
            }
            if (current->left != nil) {
                stack.push(current->left);
            }
        }
        cout << endl;
    }

    void postOrderIterative() {
        if (root == nil) {
            cout << "Tree empty" << endl;
            return;
        }
        
        Stack<Node*> stack1, stack2;
        stack1.push(root);
        
        while (!stack1.empty()) {
            Node* current = stack1.top();
            stack1.pop();
            stack2.push(current);
            
            if (current->left != nil) {
                stack1.push(current->left);
            }
            if (current->right != nil) {
                stack1.push(current->right);
            }
        }
        
        while (!stack2.empty()) {
            cout << stack2.top()->data << getColorSymbol(stack2.top()->color) << " ";
            stack2.pop();
        }
        cout << endl;
    }

    void levelOrderIterative() {
        if (root == nil) {
            cout << "Tree empty" << endl;
            return;
        }
        
        Queue<Node*> queue;
        queue.push(root);
        
        while (!queue.empty()) {
            Node* current = queue.front();
            queue.pop();
            cout << current->data << getColorSymbol(current->color) << " ";
            
            if (current->left != nil) {
                queue.push(current->left);
            }
            if (current->right != nil) {
                queue.push(current->right);
            }
        }
        cout << endl;
    }

    Node* getRoot() {
        return root;
    }

    void printTree() {
        if (root == nil) {
            cout << "Tree empty" << endl;
            return;
        }

        int height = getHeight(root);
        int maxLevel = height - 1;
        
        // Вычисляем максимальную ширину (количество элементов на последнем уровне)
        int maxWidth = (1 << maxLevel) * 4 - 3;
        
        // Создаем массив строк для вывода
        int linesCount = height * 2 - 1;
        char** lines = new char*[linesCount];
        for (int i = 0; i < linesCount; i++) {
            lines[i] = new char[maxWidth + 1];
            for (int j = 0; j < maxWidth; j++) {
                lines[i][j] = ' ';
            }
            lines[i][maxWidth] = '\0';
        }
        
        Queue<pair<Node*, pair<int, int>>> queue; 
        queue.push({root, {0, maxWidth / 2}});
        
        cout << "Red Black Tree" << endl;
        cout << "Red with square brackets, Black without brackets" << endl;
        while (!queue.empty()) {
            auto front = queue.front();
            Node* node = front.first;
            int level = front.second.first;
            int pos = front.second.second;
            queue.pop();
            
            string nodeStr = to_string(node->data) + getColorSymbol(node->color);
            if (node->color == Color::RED) {
                nodeStr = "[" + to_string(node->data) + "]" + getColorSymbol(node->color);
            } else {
                nodeStr = to_string(node->data) + getColorSymbol(node->color);
            }
            
            int startPos = pos - nodeStr.length() / 2;
            for (size_t i = 0; i < nodeStr.length(); i++) {
                lines[level * 2][startPos + i] = nodeStr[i];
            }
            
            // Вычисляем смещение для детей
            int offset = (1 << (maxLevel - level - 1));
            
            // Добавляем левого ребенка
            if (node->left != nil) {
                int leftPos = pos - offset;
                lines[level * 2 + 1][leftPos + 1] = '/';
                for (int i = leftPos + 2; i < pos; i++) {
                    lines[level * 2 + 1][i] = '-';
                }
                queue.push({node->left, {level + 1, leftPos}});
            }
            
            // Добавляем правого ребенка
            if (node->right != nil) {
                int rightPos = pos + offset;
                lines[level * 2 + 1][pos] = '-';
                for (int i = pos + 1; i < rightPos; i++) {
                    lines[level * 2 + 1][i] = '-';
                }
                lines[level * 2 + 1][rightPos - 1] = '\\';
                queue.push({node->right, {level + 1, rightPos}});
            }
        }
        
        for (int i = 0; i < linesCount; i++) {
            bool isEmpty = true;
            for (int j = 0; j < maxWidth; j++) {
                if (lines[i][j] != ' ') {
                    isEmpty = false;
                    break;
                }
            }
            if (!isEmpty) {
                cout << lines[i] << endl;
            }
        }

        for (int i = 0; i < linesCount; i++) {
            delete[] lines[i];
        }
        delete[] lines;
    }

private:
    // Вспомогательная функция для получения символа цвета
    string getColorSymbol(Color color) {
        return (color == Color::RED) ? "(R)" : "(B)";
    }

    int getHeight(Node* node) {
        if (node == nil) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    Node* searchHelper(Node* node, int data) {
        while (node != nil && data != node->data) {
            if (data < node->data) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return node;
    }

    int getSize() {
        if (root == nil) return 0;
        
        Stack<Node*> stack;
        stack.push(root);
        int size = 0;
        
        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();
            size++;
            
            if (current->right != nil) {
                stack.push(current->right);
            }
            if (current->left != nil) {
                stack.push(current->left);
            }
        }
    return size;
    }   

    void rebuildTree() {
        if (root == nil) return;
        
        int size = getSize();        
        int* values = new int[size];
        collectValues(root, values);
        
        clearIterative();
        
        for (int i=0;i<size;i++) {
            insert(values[i]);
        }
    }
    
    void collectValues(Node* node, int*& values) {
        if (node == nil) return;
        int index = 0;
        Stack<Node*> stack;
        stack.push(root);
        
        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();
            values[index] = current->data;
            index++;

            if (current->right != nil) {
                stack.push(current->right);
            }
            if (current->left != nil) {
                stack.push(current->left);
            }
        }
        cout << endl;
    }

    void clearIterative() {
        if (root == nil) return;
        
        Stack<Node*> stack;
        stack.push(root);
        
        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();
            
            if (current->left != nil) {
                stack.push(current->left);
            }
            if (current->right != nil) {
                stack.push(current->right);
            }
            
            delete current;
        }
        
        root = nil;
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        
        if (y->left != nil) {
            y->left->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        
        if (y->right != nil) {
            y->right->parent = x;
        }
        
        y->parent = x->parent;
        
        if (x->parent == nil) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        
        y->right = x;
        x->parent = y;
    }

    void insertFixup(Node* z) {
        while (z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == Color::RED) {
                    // Случай 1
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // Случай 2
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Случай 3
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == Color::RED) {
                    // Случай 4
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        // Случай 5
                        z = z->parent;
                        rightRotate(z);
                    }
                    // Случай 6
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    Node* minimum(Node* node) {
        while (node->left != nil) {
            node = node->left;
        }
        return node;
    }

    // Перемещение поддеревьев
    void transplant(Node* u, Node* v) {
        if (u->parent == nil) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNode(Node* z) {
        Node* y = z;
        Node* x = nil;
        Color yOriginalColor = y->color;
        
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        if (yOriginalColor == Color::BLACK) {
            deleteFixup(x);
        }
        
        delete z;
    }

    void deleteFixup(Node* x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }

    void clearHelper(Node* node) {
        if (node != nil) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }
};

#endif