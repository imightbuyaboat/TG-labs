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
#include "graph.h"

char* outputFileName = "default.txt";
const int16_t SRC = 0;

void createGraph(const int16_t size, const char* fileName) {
    std::ofstream file(fileName, std::ios::binary);

    file.write(reinterpret_cast<const char*>(&size), sizeof(int16_t));

    srand(static_cast<unsigned int>(time(0)));
    for (int16_t i = 0; i < size; i++) {
        for (int16_t j = 0; j < size; j++) {
            //int16_t value = rand() % (2 * INT16_MAX + 1) - INT16_MAX;
            //int16_t value = rand() % (2 * 20 + 1) - 20;

            int16_t value = rand() % 21;
            file.write(reinterpret_cast<const char*>(&value), sizeof(int16_t));
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        int16_t size = static_cast<int16_t>(std::stoi(argv[3]));
        createGraph(size, argv[2]);
        return 0;
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
        std::cerr << "Can not open file: " << argv[1] << std::endl;
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
        int16_t diam = 0, radius = INT16_MAX;

        for(size_t i = 0; i < graph.getSize(); i++) {
            int16_t max = 0;
            for(size_t j = 0; j < graph.getSize(); j++) if (paths[i][j] != INT16_MAX && paths[i][j] > max) max = paths[i][j];

            if (max > diam) diam = max;
            if (max < radius) radius = max;
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