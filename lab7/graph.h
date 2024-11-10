#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <unistd.h>
#include <set>
#include <stack>

#define vertexPair std::pair<int16_t, int16_t>

class Graph {
private:
    int16_t size;   //количество вершин
    int16_t** adjaencyMatrix; //матрица смежности

    void DFS(int16_t v, std::vector<bool> &visited, std::vector<int16_t> &component);
    
public:
    Graph(const char* fileName);
    ~Graph();
    int16_t GetSize() { return size; };

    std::vector<std::vector<int16_t>> findConnectedComponents();
    void findBridgesAndArticulationPoints(int16_t v, int16_t parent,
                                      std::vector<int16_t> &discovery, std::vector<int16_t> &low,
                                      std::vector<bool> &visited, std::vector<int16_t> &articulationPoints,
                                      std::set<std::pair<int16_t, int16_t>> &bridges, int16_t &time);
    void findBiconnectedComponents(std::vector<std::vector<int16_t>> &biconnectedComponents);
};