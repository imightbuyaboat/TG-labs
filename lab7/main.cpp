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

    std::set<std::pair<int16_t, int16_t>> bridges;
    std::vector<int16_t> articulationPoints;
    std::vector<std::vector<int16_t>> biconnectedComponents;
    std::vector<std::vector<int16_t>> connectedComponents = graph.findConnectedComponents();

    std::vector<int16_t> discovery(graph.GetSize(), -1), low(graph.GetSize(), -1);
    std::vector<bool> visited(graph.GetSize(), false);
    int16_t time = 0;

    for (int v = 0; v < graph.GetSize(); v++) {
        if (!visited[v]) {
            graph.findBridgesAndArticulationPoints(v, -1, discovery, low, visited, articulationPoints, bridges, time);
        }
    }

    graph.findBiconnectedComponents(biconnectedComponents);

    file << "Bridges:\n";
        for (const auto& bridge : bridges) {
            file << "(" << bridge.first << ", " << bridge.second << ")\n";
        }

        file << "Cut vertices:\n";
        file << "[";
        for (size_t i = 0; i < articulationPoints.size(); ++i) {
            file << articulationPoints[i];
            if (i < articulationPoints.size() - 1) {
                file << ", ";
            }
        }
        file << "]\n";

        file << "Biconnected components:\n";
        for (const auto& component : biconnectedComponents) {
            file << "[";
            for (size_t i = 0; i < component.size(); ++i) {
                file << component[i];
                if (i < component.size() - 1) {
                    file << ", ";
                }
            }
            file << "]\n";
        }

        file << "Connected components:\n";
        for (const auto& component : connectedComponents) {
            file << "[";
            for (size_t i = 0; i < component.size(); ++i) {
                file << component[i];
                if (i < component.size() - 1) {
                    file << ", ";
                }
            }
            file << "]\n";
        }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}