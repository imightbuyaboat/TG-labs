#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include <tuple>

#define MST std::vector<std::tuple<int16_t, int16_t, int16_t>>

struct Node {
    int16_t endVertex;  //конечная вершина
    int16_t weight;     //вес
    Node* next;         //указатель на следующий узел

    Node(int16_t _endVertex, int16_t _weight) : endVertex(_endVertex), weight(_weight), next(nullptr) {};
};

class Graph {
private:
    int16_t size;   //количество вершин
    Node** adjacencyList;   //список смежности

    int16_t Find(int16_t u, std::vector<int16_t>& parent);
    void Union(int16_t u, int16_t v, std::vector<int16_t>& parent, std::vector<int16_t>& rank);
    void boruvkaStep(std::vector<int16_t>& parent, std::vector<Node>& minEdges, int16_t start, int16_t end);
public:
    Graph(const char* fileName);
    ~Graph();
    void AddEdge(int16_t src, int16_t end, int16_t weight);
    Node** GetAdjacencyList();
    int16_t GetSize();
    MST Boruvka(int16_t numThreads);
};