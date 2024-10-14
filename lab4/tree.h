#include <iostream>
#include <cstdint>
#include <fstream>

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
    int32_t count = 0;

    Node* InsertNode(Node* node, int32_t x);
    bool SearchNode(Node* node, int32_t x);
    void SearchXMinNode(Node* node, int32_t* min, int32_t& ammount);
    void DeleteNode(Node* node);
public:
    Tree(const char* fileName);
    ~Tree();
    bool Insert(int32_t x);
    bool Search(int32_t x);
    int* SearchXMin(int32_t ammount);
};