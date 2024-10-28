#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <vector>
#include <math.h>

struct Cell {
    int16_t x, y;
    int16_t f;

    Cell(int16_t _x, int16_t _y, int16_t _f) : x(_x), y(_y), f(_f) {};

    bool operator<(const Cell& other) const {
        return f > other.f;
    }
};

struct PathResult {
    std::vector<Cell> path;
    int totalCost;
    float percentVisited;
};

class Graph
{
private:
    int16_t height, width;
    int16_t** matrix;

public:
    Graph(const char* fileName);
    ~Graph();
    PathResult* A( int16_t xstart, int16_t ystart, 
            int16_t xend, int16_t yend,
            int16_t (*h)(int16_t, int16_t, int16_t, int16_t), 
            bool isNeedToCheckDiagonally);
};

int16_t euclideanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);

int16_t manhattanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);

int16_t chebyshevHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);