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

const int16_t SRC = 376, END = 309;     //1
//const int16_t SRC = 168, END = 267;     //2
//const int16_t SRC = 340, END = 995;     //3
//const int16_t SRC = 477, END = 231;     //4
//const int16_t SRC = 637, END = 471;     //5
//const int16_t SRC = 93, END = 604;      //6

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
    //graph.Print();
    long maxFlow = graph.FordFalkenson(SRC, END);
    int16_t** flow = graph.GetFlow();
    Node** adjacencyList = graph.GetAdjacencyList();
    int16_t size = graph.GetSize();

    file << "Величина потока от " << SRC << " до " << END << " вершины: " << maxFlow << std::endl << std::endl;

    file << "Величина потока и пропускная способность для каждого ребра:" << std::endl;
    for(int16_t i = 0; i < size; i++) {
        Node* temp = adjacencyList[i];
        while(temp) {
            if(temp->weight == 0) {
                temp = temp->next;
                continue;
            } 
            file << "[" << i << ", " << temp->endVertex << "]: " << flow[i][temp->endVertex] << " / " << temp->weight << std::endl;
            temp = temp->next;
        }
    }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}