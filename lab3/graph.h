#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <vector>
#include <math.h>
#include <list>

struct Node {
  int16_t x, y;       // Координаты узла
  double g, h; // Стоимости: g - до текущего узла, h - эвристика, f = g + h
  Node* parent;   // Указатель на родителя для восстановления пути
  
  Node(int16_t x, int16_t y, double g = 0, double h = 0, Node* parent = nullptr)
      : x(x), y(y), g(g), h(h), parent(parent) {}
  
  bool operator>(const Node& other) const {
    return f() > other.f();
  }

  const double f() const { return g + h; }
};

struct A_Result {
  std::list<Node> path;
  size_t visitedNodes;

  A_Result(std::list<Node> path, size_t visitedNode) : 
    path(path), visitedNodes(visitedNodes) {};
};

class Graph
{
private:
    int16_t height, width;
    int16_t** matrix;

public:
    Graph(const char* fileName);
    ~Graph();
    A_Result A(Node start, Node end, 
                    int16_t (*h)(int16_t, int16_t, int16_t, int16_t), 
                    bool isNeedToCheckDiagonally);
    std::pair<int16_t, int16_t> getSizes() { return {height, width}; }
};

int16_t euclideanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);
int16_t manhattanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);
int16_t chebyshevHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend);