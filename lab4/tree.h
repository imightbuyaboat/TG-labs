#include <iostream>
#include <cstdint>
#include <fstream>
#include <iomanip>

struct Node {
    int32_t value;
    Node* left;
    Node* right;

    Node(int32_t _value) : value(_value), left(nullptr), right(nullptr) {};
};

class Tree
{
private:
    Node* root;
    int32_t count;

    Node* insertNode(Node* node, int32_t x);
    bool searchNode(Node* node, int32_t x);
    void searchXMinNode(Node* node, int32_t* min, int32_t& ammount);
    void deleteNode(Node* node);
    void printNode(Node* node, int padding);
public:
    Tree();
    ~Tree();
    void InitializeFromFile(const char* fileName); 
    bool Insert(int32_t x);
    bool Search(int32_t x);
    int* SearchXMin(int32_t ammount);
    void Print();
};