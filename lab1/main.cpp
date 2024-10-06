#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>

#define vertexPair std::pair<int16_t, int16_t>

char* defaultFileName = "default.bin";
const int16_t SRC = 0;

class Graph {

private:
    int16_t size;
    int16_t** correspondenceMatrix;
    bool negativeCycle;

public:
    Graph(const char* fileName, const bool addNewVertex) {
        FILE *archive = fopen(fileName, "rb");
        if (archive == NULL) {
            printf("Can not open archive %s\n", fileName);
            exit(0);
        }

        fread(&size, sizeof(int16_t), 1, archive);
        if(addNewVertex) size++;
        printf("Size of matrix: %d x %d\n", size, size);

        correspondenceMatrix = new int16_t*[size];
        for(size_t i = 0; i < size; i++) {
            correspondenceMatrix[i] = new int16_t[size];
        }

        for(size_t i = 0; i < size; i++) {
            for(size_t j = 0; j < size; j++) {
                if(addNewVertex && (i == size - 1 || j == size - 1)) {
                    correspondenceMatrix[i][j] = 0;
                }
                else fread(&correspondenceMatrix[i][j], sizeof(int16_t), 1, archive);
            }
        }

        fclose(archive);
        negativeCycle = true;
    }

    ~Graph() {
        for(size_t i = 0; i < size; i++) {
            delete[] correspondenceMatrix[i];
        }
        delete[] correspondenceMatrix;
    }

    int16_t& getElement(const int16_t row, const int16_t column) {
        return correspondenceMatrix[row][column];
    }

    int16_t* BellmanFord(const int16_t src) {
        int16_t* dist = new int16_t[size];
        for(size_t i = 0; i < size; i++) {
            dist[i] = INT16_MAX;
        }
        dist[src] = 0;

        std::set<vertexPair> vertexPairs;
        for(size_t i = 0; i < size; i++) {
            for(size_t j = 0; j < size; j++) {
                if(correspondenceMatrix[i][j] != 0) {
                    vertexPairs.insert({i, j});
                }
            }
        }

        for(size_t i = 0; i < size - 1; i++) {
            for(vertexPair pair : vertexPairs) {
                if(dist[pair.second] > dist[pair.first] + correspondenceMatrix[pair.first][pair.second]) {
                    dist[pair.second] = dist[pair.first] + correspondenceMatrix[pair.first][pair.second];
                }
            }
        }

        negativeCycle = false;
        for(vertexPair pair : vertexPairs) {
            if(dist[pair.second] > dist[pair.first] + correspondenceMatrix[pair.first][pair.second]) {
                negativeCycle = true;
                break;
            }
        }

        if(negativeCycle) return nullptr;
        return dist;
    }

    int16_t** Johnson(const char* fileName) {
        if(negativeCycle) return nullptr;

        Graph expandedGraph(fileName, true);
        int16_t* h = expandedGraph.BellmanFord(size);

        for(size_t i = 0; i < size + 1; i++) {
            for(size_t j = 0; j < size + 1; j++) {
                expandedGraph.getElement(i, j) = expandedGraph.getElement(i, j) + h[i] - h[j];
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        printf("Incorrect number of arguments: %d\n", argc);
        return 0;
    }

    if (argc == 4) {
        if (strcmp(argv[2], "-o") == 0) {
            defaultFileName = new char[strlen(argv[3]) + 1];
            strcpy(defaultFileName, argv[3]);
        }
        else {
            printf("Wrong flag: %s\n", argv[2]);
            return 0;
        }
    }


    
    if(argc == 4) delete[] defaultFileName;
    return 1;
}