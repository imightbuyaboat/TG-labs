#include "tree.h"

Tree::Tree() : root(nullptr) {};

//функция инициализации дерева из файла
void Tree::InitializeFromFile(const char* fileName) {
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

        if(!Insert(x)) {
            std::cerr << "Can not insert element: " << x << std::endl;
            exit(1);
        }        
    }

    file.close();
}

Tree::~Tree() {
    deleteNode(root);
}

//вернуть высоту узла
int Tree::getSize(Node* node) {
    if(!node) return 0;
    return node->height;
}

//функция обновления высоты узла на основе высот потомков
void Tree::fixSize(Node* node) {
    if(!node) return;
    node->height = 1 + getSize(node->left) + getSize(node->right);
}

//функция поворота дерева вправо
Node* Tree::rotateRight(Node* node) {
    Node* q = node->left;

    //если нет левого поддерева
    if(!q) return node;

    node->left = q->right;
    q->right = node;
    q->height = node->height;   //сохраняем текущую высоту

    fixSize(node);  //обновляем высоту узла
    return q;
}

//функция поворота дерева влево
Node* Tree::rotateLeft(Node* node) {
    Node* p = node->right;

    //если нет правого поддерева
    if(!p) return node;

    node->right = p->left;
    p->left = node;
    p->height = node->height;   //сохраняем текущую высоту
    
    fixSize(node);  //обновляем высоту узла
    return p;
}

void Tree::deleteNode(Node* node) {
    if(!node) return;

    deleteNode(node->left);
    deleteNode(node->right);

    delete node;
}

//функция вставки узла со значением x в корень поддерева
Node* Tree::insertRoot(Node* node, int32_t x) {
    //если дерево пустое
    if(!node) return new Node(x);

    if(x < node->value) {
        node->left = insertRoot(node->left, x);
        return rotateRight(node);   //выполняем поворот вправо
    }
    else if(x > node->value){
        node->right = insertRoot(node->right, x);
        return rotateLeft(node);    //выполняем поворот влево
    }
    else {
        //если значение уже существует
        return node;
    }
}

//рекурсивная функция вставки узла с учетом вероятности стать корнем
Node* Tree::insertNode(Node* node, int32_t x) {
    //если дерево пустое
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

//метод вставки значения в дерево
bool Tree::Insert(int32_t x) {
    root = insertNode(root, x);
    return root != nullptr;
}

//рекурсивная функция поиска узла
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

//метод поиска значения в дереве
bool Tree::Search(int32_t x) {
    return searchNode(root, x);
}

//рекурсивная функция поиска наименьших элементов в дереве
void Tree::searchXMinNode(Node* node, int32_t* min, int32_t& ammount) {
    if(!node) return;

    //обходим левое поддерево
    searchXMinNode(node->left, min, ammount);

    //ищем индекс максимального элемента в массиве минимальных элементов
    int32_t maxIndex = 0;
    for(int32_t i = 1; i < ammount; i++) {
        if(min[i] > min[maxIndex]) {
            maxIndex = i;
        }
    }

    //если текущее значение меньше максимального в массиве минимальных значений
    if(node->value < min[maxIndex]) {
        min[maxIndex] = node->value;
    }

    //обходим правое поддерево
    searchXMinNode(node->right, min, ammount);
}

//метод поиска наименьших элементов в дереве
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