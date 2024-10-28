#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>

struct Node {
    int16_t endVertex;
    int16_t weight;
    Node* next;

    Node(int16_t _endVertex, int16_t _weight) : endVertex(_endVertex), weight(_weight), next(nullptr) {};
};

class Graph {
private:
    int16_t size;
    Node** adjacencyList;
    int16_t** flow;

public:
    Graph(const char* fileName);
    ~Graph();
    void AddEdge(int16_t src, int16_t end, int16_t weight);
    void Print();
    bool BFS(int16_t src, int16_t end, bool* visited, int16_t* parent);
    long FordFalkenson(int16_t src, int16_t end);
    int16_t** GetFlow();
    Node** GetAdjacencyList();
    int16_t GetSize();
};