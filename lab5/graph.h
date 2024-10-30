#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <thread>
#include <vector>
//#include <set>
#include <algorithm>
#include <mutex>

struct Edge {
    int16_t u, v;
    int16_t weight;

    Edge(int16_t _u, int16_t _v, int16_t _weight) : u(_u), v(_v), weight(_weight) {};
};

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

    std::mutex minEdgesMutex;

    int16_t Find(int16_t u, std::vector<int16_t>& parent);
    void Union(int16_t u, int16_t v, std::vector<int16_t>& parent, std::vector<int16_t>& rank);
public:
    Graph(const char* fileName);
    ~Graph();
    void AddEdge(int16_t src, int16_t end, int16_t weight);
    void Print();
    Node** GetAdjacencyList();
    int16_t GetSize();
    std::vector<Edge> Boruvka();
};