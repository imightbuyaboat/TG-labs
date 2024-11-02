#include <iostream>
#include <cstdint>
#include <fstream>
#include <iomanip>

struct Node {
    int32_t value;  //значение-ключ
    int height;     //высота
    Node* left;     //левый потомок
    Node* right;    //правый потомок

    Node(int32_t _value) : value(_value), height(1), left(nullptr), right(nullptr) {};
};

class Tree
{
private:
    Node* root; //корень дерева

    int getSize(Node* node);
    void fixSize(Node* node);
    Node* insertRoot(Node* node, int32_t x);
    Node* insertNode(Node* node, int32_t x);
    bool searchNode(Node* node, int32_t x);
    void searchXMinNode(Node* node, int32_t* min, int32_t& ammount);
    void deleteNode(Node* node);
    void printNode(Node* node, int padding);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
public:
    Tree();
    ~Tree();
    void InitializeFromFile(const char* fileName); 
    bool Insert(int32_t x);
    bool Search(int32_t x);
    int* SearchXMin(int32_t ammount);
    void Print();
};