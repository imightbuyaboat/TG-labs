#include "RBTree.h"

//конструктор узла дерева
Node::Node(int32_t data) {
    this->data = data;
    color = RED;
    left = right = parent = nullptr;
}

//конструктор дерева
RBTree::RBTree() : root(nullptr) {};

//деструктор дерева
RBTree::~RBTree() {
    deleteNode(root);
}

//метод удаления узла из дерева
void RBTree::deleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }
    deleteNode(node->left);    //рекурсивно удаляем левое поддерево
    deleteNode(node->right);   //рекурсивно удаляем правое поддерево
    delete node;               //удаляем сам узел
}

//функция инициализации дерева из файла
void RBTree::InitializeFromFile(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    //считываем очередное значение и добавляем в дерево
    while(true) {
        int32_t x;
        file.read(reinterpret_cast<char*>(&x), sizeof(int32_t));

        if (file.eof()) break;

        insertValue(x);     
    }

    file.close();
}

//функция получения цвета узла
int RBTree::getColor(Node *&node) {
    //если текущий узел является листом, то возвращаем черный цвет
    return (node == nullptr) ? BLACK : node->color;
}

//функция установки цвета узла
void RBTree::setColor(Node *&node, int color) {
    if (node != nullptr) {
        node->color = color;
    }
}

//функция вставки узла в поддерево
Node* RBTree::insertNode(Node *&root, Node *&ptr) {
    //если корень текущего поддерева пуст, то добавляем новый узел на его место
    if (root == nullptr)
        return ptr;

    //добавляем узел в левое поддерево
    if (ptr->data < root->data) {
        root->left = insertNode(root->left, ptr);
        //устанавливаем родителя
        root->left->parent = root;
    }
    //добавляем узел в правое поддерево 
    else if (ptr->data > root->data) {
        root->right = insertNode(root->right, ptr);
        //устанавливаем родителя
        root->right->parent = root;
    }

    return root;
}

//операция вставки элемента в дерево
void RBTree::insertValue(int32_t n) {
    Node *node = new Node(n);
    root = insertNode(root, node);

    //исправляем балансировку дерева
    fixInsertRBTree(node);
}

//функция поворота узла влево
void RBTree::rotateLeft(Node *&ptr) {
    Node *right_child = ptr->right; //правый потомок узла
    ptr->right = right_child->left; //переносим левое поддерево правого потомка

    //обновляем родителя для нового поддерева
    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    //привязываем правого потомка к родителю узла
    right_child->parent = ptr->parent;

    //если это корень дерева, обновляем его
    if (ptr->parent == nullptr)
        root = right_child;
    //обновляем ссылку на левого потомка у родителя
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    //обновляем ссылку на правого потомка у родителя
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;    //переносим узел под правого потомка
    ptr->parent = right_child;  //обновляем родителя для узла
}

void RBTree::rotateRight(Node *&ptr) {
    Node *left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

//функция исправления структуры дерева после вставки нового узла
void RBTree::fixInsertRBTree(Node *&ptr) {
    Node *parent = nullptr;
    Node *grandparent = nullptr;

    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
        parent = ptr->parent;
        grandparent = parent->parent;

        //случай, если родитель является левым потомком
        if (parent == grandparent->left) {
            Node *uncle = grandparent->right;

            //случай 1: дядя красный
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                //случай 2: узел — правый потомок
                if (ptr == parent->right) {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }

                //случай 3: узел — левый потомок
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
        //случай, если родитель является правым потомком 
        else {
            Node *uncle = grandparent->left;

            //аналогично для правой ветки
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->left) {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }

    //корень всегда черный
    setColor(root, BLACK);
}

//функция поиска элемента в поддереве
Node* RBTree::findValue(Node *node, int32_t data) {
    if (node == nullptr || node->data == data) {
        return node;
    }
    if (data < node->data) {
        return findValue(node->left, data);
    }
    return findValue(node->right, data);
}

//операция посика элемента в дереве
Node* RBTree::findValue(int32_t data) {
    return findValue(root, data);
}

//функция поиска 10 минимальных элементов в поддереве
void RBTree::getSmallest10(Node *node, std::vector<int32_t> &smallest) {
    if(!node) return;

    //обходим левое поддерево
    getSmallest10(node->left, smallest);

    //ищем индекс максимального элемента в массиве минимальных элементов
    int32_t maxIndex = 0;
    for(int32_t i = 1; i < 10; i++) {
        if(smallest[i] > smallest[maxIndex]) {
            maxIndex = i;
        }
    }

    //если текущее значение меньше максимального в массиве минимальных значений
    if(node->data < smallest[maxIndex]) {
        smallest[maxIndex] = node->data;
    }

    //обходим правое поддерево
    getSmallest10(node->right, smallest);
}

//операция поиска 10 минимальных элементов в дереве
std::vector<int32_t> RBTree::getSmallest10() {
    std::vector<int32_t> smallest(10, INT32_MAX);

    getSmallest10(root, smallest);
    return smallest;
}
