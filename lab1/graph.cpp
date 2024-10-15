#include "graph.h"

Graph::Graph(const char* fileName, const bool addNewVertex) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    file.read(reinterpret_cast<char*>(&size), sizeof(int16_t));

    if(addNewVertex) size++;
    else std::cout << "Size of matrix: " << size << " x " << size << std::endl;

    correspondenceMatrix = new int16_t*[size];
    for(size_t i = 0; i < size; i++) {
        correspondenceMatrix[i] = new int16_t[size];
    }

    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if(addNewVertex && (i == size - 1 || j == size - 1)) {
                correspondenceMatrix[i][j] = INT16_MAX;
            }
            else file.read(reinterpret_cast<char*>(&correspondenceMatrix[i][j]), sizeof(int16_t));
        }
    }

    if(addNewVertex) {
        for(size_t i = 0; i < size; i++) correspondenceMatrix[size - 1][i] = 0;
    }

    Print();

    file.close();
}

Graph::~Graph() {
    for(size_t i = 0; i < size; i++) {
        delete[] correspondenceMatrix[i];
    }
    delete[] correspondenceMatrix;
}

int16_t* Graph::BellmanFord(const int16_t src) {
    int16_t* dist = new int16_t[size];
    for(size_t i = 0; i < size; i++) {
        dist[i] = INT16_MAX;
    }
    dist[src] = 0;

    std::set<vertexPair> vertexPairs;
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if(correspondenceMatrix[i][j] != INT16_MAX && i != j) {
                vertexPairs.insert({i, j});
            }
        }
    }

    for(vertexPair pair : vertexPairs) {
        std::cout << "[" << pair.first << "; " << pair.second << "]  ";
    }
    std::cout << std::endl;
    for(vertexPair pair : vertexPairs) {
        std::cout << std::setw(8) << correspondenceMatrix[pair.first][pair.second];
    }
    std::cout << std::endl << std::endl;

    for(size_t i = 0; i < size - 1; i++) {
        for(vertexPair pair : vertexPairs) {
            if(dist[pair.second] > dist[pair.first] + correspondenceMatrix[pair.first][pair.second]) {
                dist[pair.second] = dist[pair.first] + correspondenceMatrix[pair.first][pair.second];
            }
        }

        for(size_t i = 0; i < size; i++) std::cout << std::setw(3) << dist[i];
        std::cout << std::endl;
    }
    std::cout << std::endl;

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

void Graph::Dijkstra(const int16_t src, int16_t* distance, int16_t* previous) {
    for(size_t i = 0; i < size; i++) distance[i] = INT16_MAX;
    for(size_t i = 0; i < size; i++) previous[i] = -1;
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

int16_t** Graph::Johnson(const char* fileName) {

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
    expandedGraph.Print();

    int16_t** allPairsShortestPaths = new int16_t*[size];
    for (size_t i = 0; i < size; i++) {
        int16_t* distance = new int16_t[size];
        int16_t* previous = new int16_t[size];
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

    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            std::cout << std::setw(6) << allPairsShortestPaths[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    delete[] h;
    return allPairsShortestPaths;
}

void Graph::Print() {
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            std::cout << std::setw(6) << correspondenceMatrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}