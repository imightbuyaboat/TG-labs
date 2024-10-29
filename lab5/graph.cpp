#include "graph.h"

Graph::Graph(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    file.read(reinterpret_cast<char*>(&size), sizeof(int16_t));
    std::cout << "size: " << size << std::endl;

    adjacencyList = new Node*[size];
    for(int16_t i = 0; i < size; i++) adjacencyList[i] = nullptr;

    while(true) {
        int16_t startVertex, endVertex, weight;
        file.read(reinterpret_cast<char*>(&startVertex), sizeof(int16_t));

        if (file.eof()) break;

        file.read(reinterpret_cast<char*>(&endVertex), sizeof(int16_t));
        file.read(reinterpret_cast<char*>(&weight), sizeof(int16_t));

        if(weight == 0) continue;

        AddEdge(startVertex, endVertex, weight);

        //std::cout << "[" << startVertex << ", " << endVertex << "]: " << weight << std::endl;
    }
    //std::cout << std::endl;

    file.close();
}

Graph::Graph(int16_t _size) : size(_size) {
    adjacencyList = new Node*[_size];
    for(int16_t i = 0; i < _size; i++) adjacencyList[i] = nullptr;
}

Graph::~Graph() {
    for (int16_t i = 0; i < size; i++) {
        Node* current = adjacencyList[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] adjacencyList;
}

void Graph::AddEdge(int16_t src, int16_t end, int16_t weight) {
    Node* node = new Node(end, weight);
    node->next = adjacencyList[src];
    adjacencyList[src] = node;

    // Node* temp = adjacencyList[end];
    // bool reverseEdgeExists = false;
    // while (temp) {
    //     if (temp->endVertex == src) {
    //         reverseEdgeExists = true;
    //         break;
    //     }
    //     temp = temp->next;
    // }

    // if (!reverseEdgeExists) {
    //     node = new Node(src, 0);
    //     node->next = adjacencyList[end];
    //     adjacencyList[end] = node;
    // }
}

void Graph::Print() {
    for(size_t i = 0; i < size; i++) {
        std::cout << "w[" << i << "] = [";

        Node* temp = adjacencyList[i];
        while(temp) {
            if(temp->weight == 0) {
                temp = temp->next;
                continue;
            } 
            std::cout << temp->endVertex;
            temp = temp->next;
            if(temp && temp->weight != 0) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}

Node** Graph::GetAdjacencyList() { return adjacencyList; }
int16_t Graph::GetSize() { return size; }

int16_t Find(int16_t vertex, std::vector<int16_t>& parent) {
    if (parent[vertex] != vertex) parent[vertex] = Find(parent[vertex], parent);
    return parent[vertex];
}

void Union(int16_t u, int16_t v, std::vector<int16_t>& parent, std::vector<int16_t>& rank) {
    int16_t rootU = Find(u, parent);
    int16_t rootV = Find(v, parent);

    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV])
            parent[rootV] = rootU;
        else if (rank[rootU] < rank[rootV])
            parent[rootU] = rootV;
        else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

Graph Graph::Boruvka() {
    Graph MST(size);

    std::vector<int16_t> parent(size);
    std::vector<int16_t> rank(size, 0);
    for (int16_t i = 0; i < size; i++) parent[i] = i;

    int numComponents = size;
    std::mutex minEdgesMutex;

    while(numComponents > 1) {
        Edge** minEdges = new Edge*[size];
        std::fill(minEdges, minEdges + size, nullptr);

        auto searchMinEdge = [&](int16_t u, Node* node) {
            Node* curr = node;
            int16_t minWeight = INT16_MAX, v;
            Edge* e = nullptr;

            while(curr) {
                if(curr->weight < minWeight) {
                    minWeight = curr->weight;
                    v = curr->endVertex;
                }
                curr = curr->next;
            }

            if(minWeight == INT16_MAX) return;

            e = new Edge(u, v, minWeight);

            std::lock_guard<std::mutex> lock(minEdgesMutex);
            minEdges[u] = e;
        };

        std::vector<std::thread> threads;
        threads.reserve(size); 
        for(int16_t i = 0; i < size; i++) {
            threads.emplace_back(searchMinEdge, i, adjacencyList[i]);
        }
        for (auto& thread : threads) {
            thread.join();
        }

        for (int16_t i = 0; i < size; i++) {
            if (minEdges[i]) {
                int16_t u = minEdges[i]->u;
                int16_t v = minEdges[i]->v;
                int16_t w = minEdges[i]->weight;

                int16_t rootU = Find(u, parent);
                int16_t rootV = Find(v, parent);

                if (rootU != rootV) {
                    MST.AddEdge(u, v, w);
                    Union(rootU, rootV, parent, rank);
                    numComponents--;

                    std::cout<<numComponents<<std::endl;
                }
                std::cout<<std::endl;
            }
        }

        for (int16_t i = 0; i < size; i++) {
            delete minEdges[i];
        }
        delete[] minEdges;

        
    }

    return MST;
}