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

    Graph graph(argv[1]);

    auto matchingEdges = graph.MaximumBipartiteMatching();
    if(!matchingEdges.empty()) {
        file << "Граф является двудольным" << std::endl;

        int64_t matchingSize = 0;
        for(auto& edge : matchingEdges) {
            matchingSize++;
        }
        file << "Число паросочетаний: " << matchingSize << std::endl;

        for(auto& edge : matchingEdges) {
            file << "(" << edge.first << ", " << edge.second << ")" << std::endl;
        }
    }
    else {
        file << "Граф не является двудольным";
    }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}