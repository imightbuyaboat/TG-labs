#include <iostream>
#include <cstdint>
#include <fstream>
#include <queue>
#include <vector>
#include <math.h>
#include <list>

struct Node {
  int16_t x, y; //координаты узла
  double g, h;  //стоимости: g - до текущего узла, h - эвристика
  Node* parent; //указатель на родителя для восстановления пути
  
  Node(int16_t _x, int16_t _y, double _g = 0, double _h = 0, Node* _parent = nullptr)
      : x(_x), y(_y), g(_g), h(_h), parent(_parent) {}
  
  bool operator>(const Node& other) const {
    return f() > other.f();
  }

  const double f() const { return g + h; }
};

struct A_Result {
  std::list<Node> path; //найденный путь
  size_t visitedNodes;  //количество посещенных клеток

  A_Result(std::list<Node> _path, size_t _visitedNodes) : 
    path(_path), visitedNodes(_visitedNodes) {};
};

class Graph
{
private:
    int16_t height, width;  //размеры графа
    int16_t** matrix;       //матрица клеток

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