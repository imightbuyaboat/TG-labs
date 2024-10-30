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

        //if(weight == 0 || weight == INT16_MAX) continue;

        AddEdge(startVertex, endVertex, weight);

        //std::cout << "[" << startVertex << ", " << endVertex << "]: " << weight << std::endl;
    }
    //std::cout << std::endl;

    file.close();
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

    // node = new Node(src, weight);
    // node->next = adjacencyList[end];
    // adjacencyList[end] = node;
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

int16_t Graph::Find(int16_t u, std::vector<int16_t>& parent) {
    if (parent[u] != u) parent[u] = Find(parent[u], parent);
    return parent[u];
}

void Graph::Union(int16_t u, int16_t v, std::vector<int16_t>& parent, std::vector<int16_t>& rank) {
    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

std::vector<Edge> Graph::Boruvka() {
    std::vector<Edge> mstEdges;
    std::vector<int16_t> minEdge(size, -1);
    std::vector<int16_t> parent(size);
    std::vector<int16_t> rank(size, 0);
    for (int16_t i = 0; i < size; i++) parent[i] = i;

    int16_t numComponents = size;

    while(numComponents > 1) {
        std::fill(minEdge.begin(), minEdge.end(), -1);

        std::vector<std::thread> threads;
        for (int16_t u = 0; u < size; u++) {
            threads.emplace_back([&, u]() {
                Node* curr = adjacencyList[u];
                while (curr) {
                    int16_t setU = Find(u, parent);
                    int16_t setV = Find(curr->endVertex, parent);

                    if (setU != setV) {
                        std::lock_guard<std::mutex> lock(minEdgesMutex);
                        if (minEdge[setU] == -1 || adjacencyList[minEdge[setU]]->weight > curr->weight) {
                            minEdge[setU] = u; 
                        }
                    }
                    curr = curr->next;
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        for (int16_t i = 0; i < size; i++) {
            if (minEdge[i] != -1) {
                Node* e = adjacencyList[minEdge[i]];
                int16_t setU = Find(minEdge[i], parent);
                int16_t setV = Find(e->endVertex, parent);

                if (setU != setV) {
                    mstEdges.emplace_back(minEdge[i], e->endVertex, e->weight);
                    Union(setU, setV, parent, rank);
                    numComponents--;
                }
            }
        }
        std::cout<<numComponents<<std::endl;
        // for (const auto& edge : mstEdges) {
        //     std::cout << "(" << edge.u << ", " << edge.v << ", " << edge.weight << ")";
        // }    
        // std::cout<<std::endl;
    }

    return mstEdges;
}