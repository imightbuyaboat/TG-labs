#include "tree.h"

Tree::Tree() : root(nullptr) {};

void Tree::InitializeFromFile(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    while(true) {
        int32_t x;
        file.read(reinterpret_cast<char*>(&x), sizeof(int32_t));

        if (file.eof()) break;

        if(!Insert(x)) {
            std::cerr << "Can not insert element: " << x << std::endl;
            exit(1);
        }

        //std::cout << "[" << startVertex << ", " << endVertex << "]: " << weight << std::endl;
    }

    file.close();
}

Tree::~Tree() {
    deleteNode(root);
}

int Tree::getSize(Node* node) {
    if(!node) return 0;
    return node->height;
}

void Tree::fixSize(Node* node) {
    if(!node) return;
    node->height = 1 + getSize(node->left) + getSize(node->right);
}

Node* Tree::rotateRight(Node* node) {
    Node* q = node->left;
    if(!q) return node;

    node->left = q->right;
    q->right = node;
    q->height = node->height;

    fixSize(node);
    return q;
}
Node* Tree::rotateLeft(Node* node) {
    Node* p = node->right;
    if(!p) return node;

    node->right = p->left;
    p->left = node;
    p->height = node->height;
    
    fixSize(node);
    return p;
}

void Tree::deleteNode(Node* node) {
    if(!node) return;

    deleteNode(node->left);
    deleteNode(node->right);

    delete node;
}

Node* Tree::insertRoot(Node* node, int32_t x) {
    if(!node) return new Node(x);

    if(x < node->value) {
        node->left = insertRoot(node->left, x);
        return rotateRight(node);
    }
    else if(x > node->value){
        node->right = insertRoot(node->right, x);
        return rotateLeft(node);
    }
    else {
        return node;
    }
}

Node* Tree::insertNode(Node* node, int32_t x) {
    if(!node) {
        return new Node(x);
    }

    if(rand() % (node->height + 1) == 0) {
        return insertRoot(node, x);
    }

    if(x < node->value) {
        node->left = insertNode(node->left, x);
    }
    else if(x > node->value) {
        node->right = insertNode(node->right, x);
    }
    else {
        return node;
    }

    fixSize(node);
    return node;
}

bool Tree::Insert(int32_t x) {
    root = insertNode(root, x);
    return root != nullptr;
}

bool Tree::searchNode(Node* node, int32_t x) {
    if(!node) return false;

    if(node->value == x) {
        return true;
    }

    if(x < node->value) {
        return searchNode(node->left, x);
    }
    else {
        return searchNode(node->right, x);
    }
}

bool Tree::Search(int32_t x) {
    return searchNode(root, x);
}

void Tree::searchXMinNode(Node* node, int32_t* min, int32_t& ammount) {
    if(!node) return;

    searchXMinNode(node->left, min, ammount);

    int32_t maxIndex = 0;
    for(int32_t i = 1; i < ammount; i++) {
        if(min[i] > min[maxIndex]) {
            maxIndex = i;
        }
    }

    if(node->value < min[maxIndex]) {
        min[maxIndex] = node->value;
    }

    searchXMinNode(node->right, min, ammount);
}

int* Tree::SearchXMin(int32_t ammount) {

    int32_t* min = new int32_t[ammount];
    for(size_t i = 0; i < ammount; i++) min[i] = INT32_MAX;

    searchXMinNode(root, min, ammount);
    return min;
}

void Tree::printNode(Node* node, int padding) {
    if (node) {
        printNode(node->right, padding + 1);
        for (int i = 0; i < padding; i++)  std::cout << "  ";
        std::cout << node->value << std::endl;
        printNode(node->left, padding + 1);
    }
}

void Tree::Print() {
    printNode(root, 0);
}