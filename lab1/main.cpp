#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <set>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#define vertexPair std::pair<int16_t, int16_t>

char* defaultFileName = "default.txt";
const int16_t SRC = 0;

class Graph {

private:
    int16_t size;
    int16_t** correspondenceMatrix;

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

    int16_t getSize() { return size; }

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

        bool negativeCycle = false;
        for(vertexPair pair : vertexPairs) {
            if(dist[pair.second] > dist[pair.first] + correspondenceMatrix[pair.first][pair.second]) {
                negativeCycle = true;
                break;
            }
        }

        if(negativeCycle) return nullptr;
        return dist;
    }

    void Dijkstra(const int16_t src, std::vector<int16_t>& distance, std::vector<int16_t>& previous) {
        distance.assign(size, INT16_MAX);
        previous.assign(size, -1);
        distance[src] = 0;

        std::priority_queue<vertexPair, std::vector<vertexPair>, std::greater<vertexPair>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int16_t u = pq.top().second;
            pq.pop();

            for (int16_t v = 0; v < size; v++) {
                if (correspondenceMatrix[u][v] != INT16_MAX) {
                    int16_t alt = distance[u] + correspondenceMatrix[u][v];
                    if (alt < distance[v]) {
                        distance[v] = alt;
                        previous[v] = u;
                        pq.push({distance[v], v});
                    }
                }
            }
        }
    }

    int16_t** Johnson(const char* fileName) {

        Graph expandedGraph(fileName, true);
        int16_t* h = expandedGraph.BellmanFord(size);
        if(!h) return nullptr;

        for(size_t i = 0; i < size; i++) {
            for(size_t j = 0; j < size; j++) {
                if (expandedGraph.getElement(i, j) != INT16_MAX) {
                    expandedGraph.getElement(i, j) += h[i] - h[j];
                }
            }
        }

        int16_t** allPairsShortestPaths = new int16_t*[size];
        for (size_t i = 0; i < size; i++) {
            std::vector<int16_t> distance(size);
            std::vector<int16_t> previous(size);
            expandedGraph.Dijkstra(i, distance, previous);

            allPairsShortestPaths[i] = new int16_t[size];
            for (size_t j = 0; j < size; j++) {
                if (distance[j] != INT16_MAX) {
                    allPairsShortestPaths[i][j] = distance[j] + h[j] - h[i];
                } else {
                    allPairsShortestPaths[i][j] = INT16_MAX;
                }
            }
        }

        delete[] h;
        return allPairsShortestPaths;
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

    std::ofstream file(argv[1]);
    if (!file) {
        std::cerr << "Can not open file: " << argv[1] << std::endl;
        return 0;
    }

    Graph graph(defaultFileName, false);

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

    int16_t **paths = graph.Johnson(defaultFileName);
    if (paths) {

    }

    file.close();
    if(argc == 4) delete[] defaultFileName;
    return 1;
}