#include "tree.h"

Tree::Tree() : root(nullptr), count(0) {};

void Tree::InitializeFromFile(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    int32_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(int32_t));
    std::cout << "size: " <<size << std::endl;

    for(int32_t i = 0; i < size; i++) {
        int32_t x;
        file.read(reinterpret_cast<char*>(&x), sizeof(int32_t));
        std::cout << x << " ";
        
        if(!Insert(x)) {
            std::cerr << "Can not insert element: " << x << std::endl;
            exit(1);
        }
    }

    file.close();
}

Tree::~Tree() {
    DeleteNode(root);
}

void Tree::DeleteNode(Node* node) {
    if(!node) return;

    DeleteNode(node->left);
    DeleteNode(node->right);

    delete node;
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

void Tree::PrintNode(Node* node, int padding) {
    if (node) {
        PrintNode(node->right, padding + 1);
        for (int i = 0; i < padding; i++)  std::cout << "  ";
        std::cout << node->value << std::endl;
        PrintNode(node->left, padding + 1);
    }
}

void Tree::Print() {
    PrintNode(root, 0);
}