#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>

struct Node {
    int16_t endVertex;  //конечная вершина
    int16_t weight;     //вес
    Node* next;         //следующий узел

    Node(int16_t _endVertex, int16_t _weight) : endVertex(_endVertex), weight(_weight), next(nullptr) {};
};

class Graph {
private:
    int16_t size;           //число вершин
    Node** adjacencyList;   //список указателей на узлы
    int16_t** flow;         //матрица потоков

public:
    Graph(const char* fileName);
    ~Graph();
    void AddEdge(int16_t src, int16_t end, int16_t weight);
    bool BFS(int16_t src, int16_t end, bool* visited, int16_t* parent);
    long FordFalkenson(int16_t src, int16_t end);
    int16_t** GetFlow();
    Node** GetAdjacencyList();
    int16_t GetSize();
};