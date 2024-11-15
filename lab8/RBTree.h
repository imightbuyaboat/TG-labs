#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>

enum Color {RED, BLACK, DOUBLE_BLACK};

struct Node {
    int32_t data;
    int color;
    Node *left, *right, *parent;

    Node(int32_t data);
};

class RBTree {
    Node *root;
public:
    RBTree();
    ~RBTree();
    void InitializeFromFile(const char* fileName);
    
    void insertValue(int32_t n);
    Node* findValue(int32_t data);                  
    std::vector<int32_t> getSmallest10();                     
    
private:
    int getColor(Node *&node);
    void setColor(Node *&node, int color);
    
    void rotateLeft(Node *&ptr);
    void rotateRight(Node *&ptr);
    void fixInsertRBTree(Node *&ptr);
    
    void deleteNode(Node* node);
    Node* insertNode(Node *&root, Node *&ptr);
    Node* findValue(Node *node, int32_t data);      
    void getSmallest10(Node *node, std::vector<int32_t> &smallest);
};