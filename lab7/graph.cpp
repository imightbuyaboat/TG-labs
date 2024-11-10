#include "graph.h"

Graph::Graph(const char* fileName) {
    //открываем бинарный файл для чтения
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    //считываем количество вершин
    file.read(reinterpret_cast<char*>(&size), sizeof(int16_t));
    std::cout << "Size of matrix: " << size << " x " << size << std::endl;

    //выделяем память под матрицу смежности
    adjaencyMatrix = new int16_t*[size];
    for(size_t i = 0; i < size; i++) {
        adjaencyMatrix[i] = new int16_t[size];
    }

    //заполняем матрицу смежности из файла
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            file.read(reinterpret_cast<char*>(&adjaencyMatrix[i][j]), sizeof(int16_t));
        }
    }

    file.close();
}

Graph::~Graph() {
    for(size_t i = 0; i < size; i++) {
        delete[] adjaencyMatrix[i];
    }
    delete[] adjaencyMatrix;
}

void Graph::DFS(int16_t v, std::vector<bool> &visited, std::vector<int16_t> &component) {
    visited[v] = true;
    component.push_back(v);
    for (int16_t u = 0; u < size; ++u) {
        if (adjaencyMatrix[v][u] && !visited[u]) {
            DFS(u, visited, component);
        }
    }
}

std::vector<std::vector<int16_t>> Graph::findConnectedComponents() {
    std::vector<bool> visited(size, false);
    std::vector<std::vector<int16_t>> components;

    for (int16_t v = 0; v < size; ++v) {
        if (!visited[v]) {
            std::vector<int16_t> component;
            DFS(v, visited, component);
            components.push_back(component);
        }
    }
    return components;
}

void Graph::findBridgesAndArticulationPoints(int16_t v, int16_t parent,
                                      std::vector<int16_t> &discovery, std::vector<int16_t> &low,
                                      std::vector<bool> &visited, std::vector<int16_t> &articulationPoints,
                                      std::set<std::pair<int16_t, int16_t>> &bridges, int16_t &time) {
    visited[v] = true;
    discovery[v] = low[v] = ++time;
    int16_t children = 0;

    for (int16_t u = 0; u < size; ++u) {
        if (!adjaencyMatrix[v][u]) continue;

        if (!visited[u]) {
            children++;
            findBridgesAndArticulationPoints(u, v, discovery, low, visited,
                                             articulationPoints, bridges, time);

            low[v] = std::min(low[v], low[u]);

            if (low[u] > discovery[v]) {
                bridges.insert({std::min(v, u), std::max(v, u)});
            }
            if (parent == -1 && children > 1) {
                articulationPoints.push_back(v);
            } else if (parent != -1 && low[u] >= discovery[v]) {
                articulationPoints.push_back(v);
            }
        } else if (u != parent) {
            low[v] = std::min(low[v], discovery[u]);
        }
    }
}

void Graph::findBiconnectedComponents(std::vector<std::vector<int16_t>> &biconnectedComponents) {
    std::vector<int16_t> discovery(size, -1), low(size, -1);
    std::vector<bool> visited(size, false);
    std::stack<std::pair<int16_t, int16_t>> edgeStack;
    int16_t time = 0;

    auto dfsBCC = [&](auto&& dfsBCC, int16_t v, int16_t parent) -> void {
        visited[v] = true;
        discovery[v] = low[v] = ++time;
        int16_t children = 0;

        for (int16_t u = 0; u < size; ++u) {
            if (!adjaencyMatrix[v][u]) continue;

            if (!visited[u]) {
                children++;
                edgeStack.push({v, u});
                dfsBCC(dfsBCC, u, v);

                low[v] = std::min(low[v], low[u]);

                if ((parent == -1 && children > 1) || (parent != -1 && low[u] >= discovery[v])) {
                    std::vector<int16_t> component;
                    while (edgeStack.top() != std::make_pair(v, u)) {
                        component.push_back(edgeStack.top().second);
                        edgeStack.pop();
                    }
                    component.push_back(edgeStack.top().second);
                    component.push_back(edgeStack.top().first);
                    edgeStack.pop();
                    biconnectedComponents.push_back(component);
                }
            } else if (u != parent && discovery[u] < discovery[v]) {
                edgeStack.push({v, u});
                low[v] = std::min(low[v], discovery[u]);
            }
        }
    };

    for (int16_t v = 0; v < size; ++v) {
        if (!visited[v]) dfsBCC(dfsBCC, v, -1);
    }
}