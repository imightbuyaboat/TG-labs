#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "graph.h"

char* outputFileName = "default.txt";
const int16_t SRC = 0;

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

    Graph graph(argv[1], false);

    int16_t *dist = graph.BellmanFord(SRC);
    if(!dist) {
        file << "Граф содержит цикл с отрицательным весом";
    }
    else {
        file << "Вектор расстояний: ";
        for(size_t i = 0; i < graph.getSize(); i++) {
            if(dist[i] == INT16_MAX) file << "INF ";
            else file << dist[i] << " "; 
        }
        file << std::endl;
    }

    int16_t **paths = graph.Johnson(outputFileName);
    if (paths) {
        int16_t diam = 0, radius = INT16_MAX;

        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] > max) max = paths[i][j];

            if (max > diam) diam = max;
            if (max < radius) radius = max;
        }

        file << "Диаметр: " << diam << std::endl;
        file << "Радиус: " << radius << std::endl;

        file << "Множество центральных вершин: ";
        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] > max) max = paths[i][j];

            if (max == radius) file << i << " ";
        }
        
        file << std::endl << "Множество периферийных вершин: ";
        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] > max) max = paths[i][j];

            if (max == diam) file << i << " ";
        }
    }

    file.close();
    if(argc == 4) delete[] outputFileName;
    if(paths) {
        for(size_t i = 0; i < graph.getSize(); i++) {
            delete[] paths[i];
        }
        delete[] paths;
    }

    return 0;
}