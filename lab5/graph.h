#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <thread>
#include <vector>
//#include <set>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <tuple>

#define MST std::vector<std::tuple<int16_t, int16_t, int16_t>>

// struct BoruvkaResult {
//     MST mst; // Для хранения рёбер остовного дерева
//     int64_t totalWeight;

//     BoruvkaResult(MST _mst, int64_t _totalWeight) : mst(_mst), totalWeight(_totalWeight) {};
// };

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
    void boruvkaStep(std::vector<int16_t>& parent, std::vector<Node>& minEdges, int16_t start, int16_t end);
public:
    Graph(const char* fileName);
    ~Graph();
    void AddEdge(int16_t src, int16_t end, int16_t weight);
    void Print();
    Node** GetAdjacencyList();
    int16_t GetSize();
    MST Boruvka(int16_t numThreads);
};