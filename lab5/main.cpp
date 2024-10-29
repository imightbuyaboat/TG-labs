#include <cstring>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "graph.h"

char* outputFileName = "default.txt";

//const int16_t SRC = 376, END = 309;     //1
// const int16_t SRC = 168, END = 267;     //2
//const int16_t SRC = 340, END = 995;     //3
//const int16_t SRC = 477, END = 231;     //4 -
//const int16_t SRC = 637, END = 471;     //5
const int16_t SRC = 93, END = 604;      //6 -

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    if (argc == 4) {
        if (strcmp(argv[2], "-o") == 0) {
            outputFileName = new char[strlen(argv[3]) + 1];
            strcpy(outputFileName, argv[3]);
        }
        else {
            std::cerr << "Wrong flag: " << argv[2] << std::endl;
            return 1;
        }
    }

    std::ofstream file(outputFileName);
    if (!file) {
        std::cerr << "Can not open file: " << argv[1] << std::endl;
        return 1;
    }

    Graph graph(argv[1]);
    Graph MST = graph.Boruvka();

    int16_t size = MST.GetSize();
    Node** adjacencyList = MST.GetAdjacencyList();

    int totalWeight = 0;
    std::vector<Edge> mstEdges;
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));

    for (int16_t i = 0; i < size; i++) {
        Node* current = adjacencyList[i];
        while (current != nullptr) {
            int16_t u = i;
            int16_t v = current->endVertex;
            int16_t weight = current->weight;

            if (!visited[u][v]) {
                totalWeight += weight;
                mstEdges.push_back(Edge(u, v, weight)); 
                visited[u][v] = visited[v][u] = true;
            }
            current = current->next; 
        }
    }

    file << "Вес минимального остовного дерева: " << totalWeight << std::endl;
    file << "Рёбра минимального остовного дерева:" << std::endl;
    for (const auto& edge : mstEdges) {
        file << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")" << std::endl;
    }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}