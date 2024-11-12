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
    //функция инициализации дерева из файла
    void InitializeFromFile(const char* fileName);

    void deleteNode(Node* node);
    
    void insertValue(int32_t n);
    
    Node* findValue(int32_t data);                   // поиск элемента
    std::vector<int32_t> getSmallest10();                      // вывод 10 наименьших элементов
    
private:
    int getColor(Node *&node);
    void setColor(Node *&node, int color);
    
    Node* insertBST(Node *&root, Node *&ptr);
    
    void rotateLeft(Node *&ptr);
    void rotateRight(Node *&ptr);
    void fixInsertRBTree(Node *&ptr);
    void fixDeleteRBTree(Node *&node);
    
    Node* minValueNode(Node *&node);
    Node* maxValueNode(Node *&node);
    
    int getBlackHeight(Node *node);
    
    Node* findValue(Node *node, int32_t data);       // рекурсивная версия поиска
    void getSmallest10(Node *node, std::vector<int32_t> &smallest); // рекурсивная версия поиска 10 наименьших
};