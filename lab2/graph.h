#include <iostream>
#include <cstdint>
#include <fstream>

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
    bool DFS(int16_t src, int16_t end, bool* visited, int16_t* parent);
    int16_t FordFalkenson(int16_t src, int16_t end);
    int16_t** GetFlow();
    Node** GetAdjacencyList();
    int16_t GetSize();
};