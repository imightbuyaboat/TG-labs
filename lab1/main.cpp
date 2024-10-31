#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime> 
#include <algorithm>
#include "graph.h"

char* outputFileName = "default.txt";
const int16_t SRC = 0;

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    //меняем название выходного файла
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
        std::cerr << "Can not open file: " << outputFileName << std::endl;
        return 1;
    }

    Graph graph(argv[1], false);

    int16_t *dist = graph.BellmanFord(SRC);
    if(!dist) {
        file << "Граф содержит цикл с отрицательным весом";
    }
    else {
        file << "Вектор расстояний:\n";
        for(size_t i = 0; i < graph.getSize(); i++) {
            file << SRC << " - " << i << ": ";
            if(dist[i] == INT16_MAX) file << "∞" << std::endl;
            else file << dist[i] << std::endl; 
        }
        file << std::endl;

        delete[] dist;
    }

    int16_t **paths = graph.Johnson(argv[1]);
    if (paths) {
        int16_t* eccentricities = new int16_t[graph.getSize()];
        for(size_t i = 0; i < graph.getSize(); i++) eccentricities[i] = 0;

        int16_t diam = 0, radius = INT16_MAX;

        for(size_t i = 0; i < graph.getSize(); i++) {
            for(size_t j = 0; j < graph.getSize(); j++) {
                if (paths[i][j] != INT16_MAX) {
                    if(paths[i][j] > eccentricities[i]) eccentricities[i] = paths[i][j];
                }
            }

            if(eccentricities[i] != 0 && eccentricities[i] != INT16_MAX) {
                if(eccentricities[i] > diam) diam = eccentricities[i];
                if(eccentricities[i] < radius) radius = eccentricities[i];
            }
        }



        file << "Диаметр: " << diam << std::endl;
        file << "Радиус: " << radius << std::endl;

        file << "Множество центральных вершин: ";
        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] != INT16_MAX && paths[i][j] > max) max = paths[i][j];

            if (max == radius) file << i << " ";
        }
        
        file << std::endl << "Множество периферийных вершин: ";
        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] != INT16_MAX && paths[i][j] > max) max = paths[i][j];

            if (max == diam) file << i << " ";
        }

        for(size_t i = 0; i < graph.getSize(); i++) {
            delete[] paths[i];
        }
        delete[] paths;
    }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}