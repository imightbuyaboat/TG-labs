#include <iostream>

struct node {
    int32_t value;
    node* left;
    node* right;
};

class Tree
{
private:
    node* root;
    int32_t count = 0;
public:
    Tree(const char* fileName);
    ~Tree();
    int Add(int32_t x);
};

Tree::Tree(const char* fileName) {

}

Tree::~Tree() {
}

int Tree::Add(int32_t x) {
    node* newNode = new node;
    newNode->value = x;
    newNode->left = nullptr;
    newNode->right = nullptr;

    if (!root) {
        root = newNode;
        return 1;
    }
    
    node* curr = root;
    while(1) {
        if(x < curr->value) {
            if(!curr->left) {
                curr->left = newNode;
                break;
            } 
            else curr = curr->left; 
        }
        else if(x > curr->value) {
            if(!curr->right) {
                curr->right = newNode;
                break;
            } 
            else curr = curr->right; 
        }
        else {
            delete newNode;
            return 0;
        }
    }

    return 1;
}