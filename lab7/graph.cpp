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

bool Graph::DFS(int16_t u, std::vector<bool>& visited, std::vector<int16_t>& match) {
    visited[u] = true;

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

void Graph::Tarjan() {
    std::vector<bool> visited(size, false);
    
}