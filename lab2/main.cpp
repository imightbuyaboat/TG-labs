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
const int16_t SRC = 0, END = 3;

void createGraph(const int16_t size, const char* fileName) {
    std::ofstream file(fileName, std::ios::binary);

    file.write(reinterpret_cast<const char*>(&size), sizeof(int16_t));

    srand(static_cast<unsigned int>(time(0)));
    for (int16_t i = 0; i < size; i++) {
        for (int16_t j = 0; j < size; j++) {
            int16_t value;
            if(i == j) value = 0;
            else value = rand() % 21;
            file.write(reinterpret_cast<const char*>(&i), sizeof(int16_t));
            file.write(reinterpret_cast<const char*>(&j), sizeof(int16_t));
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

    Graph graph(argv[1]);
    graph.Print();
    graph.FordFalkenson(SRC, END);

    return 0;
}