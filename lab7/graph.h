#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <unistd.h>
#include <set>

#define vertexPair std::pair<int16_t, int16_t>

class Graph {
private:
    int16_t size;   //количество вершин
    int16_t** adjaencyMatrix; //матрица смежности

    bool DFS(int16_t u, std::vector<bool>& visited, std::vector<int16_t>& match);
    
public:
    Graph(const char* fileName);
    ~Graph();

    void Tarjan();
};