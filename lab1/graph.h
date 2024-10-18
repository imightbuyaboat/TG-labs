#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>
//#include <vector>
#include <queue>
#include <iomanip>
#include <unistd.h>

#define vertexPair std::pair<int16_t, int16_t>

class Graph {
private:
    int16_t size;   //количество вершин
    int16_t** correspondenceMatrix; //матрица смежности

public:
    Graph(const char* fileName, const bool addNewVertex);
    ~Graph();

    int16_t& getElement(const int16_t row, const int16_t column) { return correspondenceMatrix[row][column]; }
    int16_t getSize() { return size; }

    int16_t* BellmanFord(const int16_t src);
    void Dijkstra(const int16_t src, int16_t* distance);
    int16_t** Johnson(const char* fileName);

    void Print();
};