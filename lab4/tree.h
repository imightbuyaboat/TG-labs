#include <iostream>
#include <cstdint>

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
public:
    Tree(const char* fileName);
    ~Tree();
    bool Insert(int32_t x);
    bool Search(int32_t x);
    int* SearchXMin(int32_t ammount);
};

Tree::Tree(const char* fileName) {

}

Tree::~Tree() {
}

Node* Tree::InsertNode(Node* node, int32_t x) {
    if(!node) {
        count++;
        return new Node(x);
    }

    if(x < node->value) {
        node->left = InsertNode(node->left, x);
    }
    else if(x > node->value) {
        node->right = InsertNode(node->right, x);
    }
    else {
        return node;
    }

    return node;
}

bool Tree::Insert(int32_t x) {
    root = InsertNode(root, x);
    return root != nullptr;
}

bool Tree::SearchNode(Node* node, int32_t x) {
    if(!node) return false;

    if(node->value == x) {
        return true;
    }

    if(x < node->value) {
        return SearchNode(node->left, x);
    }
    else {
        return SearchNode(node->right, x);
    }
}

bool Tree::Search(int32_t x) {
    return SearchNode(root, x);
}

void Tree::SearchXMinNode(Node* node, int32_t* min, int32_t& ammount) {
    if(!node) return;

    SearchXMinNode(node->left, min, ammount);

    int32_t maxIndex = 0;
    for(int32_t i = 1; i < ammount; i++) {
        if(min[i] > min[maxIndex]) {
            maxIndex = i;
        }
    }

    if(node->value < min[maxIndex]) {
        min[maxIndex] = node->value;
    }

    SearchXMinNode(node->right, min, ammount);
}

int* Tree::SearchXMin(int32_t ammount) {
    if(count < ammount) return nullptr;

    int32_t* min = new int32_t[ammount];
    for(size_t i = 0; i < ammount; i++) min[i] = INT32_MAX;

    SearchXMinNode(root, min, ammount);
    return min;
}