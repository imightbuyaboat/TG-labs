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

//функция определения двудольности графа
bool Graph::isBipartite() {
    //массив цветов вершин (-1 - вершина не окрашена)
    std::vector<int16_t> color(size, -1);

    //проверяем каждый компонент графа, чтобы покрыть несвязные графы
    for (int16_t start = 0; start < size; start++) {
        if (color[start] != -1) continue; //вершина уже окрашена

        //начинаем BFS с вершины `start`
        std::queue<int16_t> q;
        q.push(start);
        color[start] = 0; //начинаем с цвета 0

        while (!q.empty()) {
            int16_t u = q.front();
            q.pop();

            //проверяем всех соседей вершины u
            for (int16_t v = 0; v < size; ++v) {
                //если вершина v смежна с u
                if (adjaencyMatrix[u][v] == 1) {
                    //если вершина v не окрашена, окрашиваем в противоположный цвет
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);

                    //если вершина v уже окрашена тем же цветом, что и u, граф не двудолен
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

//функция для поиска увеличивающего пути в двудольном графе
bool Graph::DFS(int16_t u, std::vector<bool>& visited, std::vector<int16_t>& match) {
    //отмечаем вершину u как посещённую
    visited[u] = true;

    //проверяем всех соседей вершины u
    for(int16_t v = 0; v < size; v++) {
        if(adjaencyMatrix[u][v] == 1) {
            if (match[v] == -1 || (!visited[match[v]] && DFS(match[v], visited, match))) {
                match[u] = v;
                match[v] = u;
                return true;
            }
        }
    }

    return false;
}

//функция для поиска максимального паросочетания в двудольном графе
std::set<std::pair<int16_t, int16_t>> Graph::MaximumBipartiteMatching() {
    //если граф не двудолен
    if(!isBipartite()) return {};

    //массив для хранения текущего сопоставления (-1 - отсутствие сопоставления)
    std::vector<int16_t> match(size, -1);
    std::vector<bool> visited(size, false);

    //для каждой вершины ищем увеличивающий путь
    for(int16_t u = 0; u < size; u++) {
        if(match[u] == -1) {
            std::fill(visited.begin(), visited.end(), false);
            DFS(u, visited, match);
        }
    }

    //сохраняем рёбра максимального паросочетания
    std::set<std::pair<int16_t, int16_t>> matchingEdges;
    for(int16_t u = 0; u < size; u++) {
        if(u < match[u]) {
            matchingEdges.insert({u, match[u]});
        } 
    }

    return matchingEdges;
}