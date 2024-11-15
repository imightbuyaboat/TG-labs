#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <unistd.h>
#include <set>
#include <list>
#include <queue>
#include <algorithm>

#define vertexPair std::pair<int16_t, int16_t>

class Graph {
private:
    int16_t size;   //количество вершин
    int16_t** adjaencyMatrix; //матрица смежности

    void bfs(size_t to, std::vector<bool>& visited, std::vector<size_t>& component);

    std::vector<size_t> findBiconn(std::list<size_t>& stack, size_t from, size_t to);

    void dfs(size_t from, size_t parent, std::vector<bool>& visited, std::vector<size_t>& disc,
           std::vector<size_t>& low, std::vector<size_t>& parentArr, std::set<std::pair<size_t, size_t>>& bridges,
           std::set<size_t>& aps, std::vector<std::vector<size_t>>& biconnComp, std::list<size_t>& stack);
    
public:
    Graph(const char* fileName);
    ~Graph();

    std::vector<std::vector<size_t>> findConnectedComponents();
    void findBridgeAps(std::set<std::pair<size_t, size_t>>& bridges,
                        std::set<size_t>& aps,
                        std::vector<std::vector<size_t>>& biconnComp);
};