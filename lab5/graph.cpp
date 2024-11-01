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

    node = new Node(src, weight);
    node->next = adjacencyList[end];
    adjacencyList[end] = node;
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

void Graph::boruvkaStep(std::vector<int16_t>& parent, std::vector<Node>& minEdges, int16_t start, int16_t end) {
    for (int16_t u = start; u < end; ++u) {
        int16_t compU = Find(u, parent);
        Node* node = adjacencyList[u];
        while(node) {
            int16_t v = node->endVertex;
            int16_t weight = node->weight;
            int16_t compV = Find(v, parent);

            if (compU != compV && weight < minEdges[compU].weight) {
                minEdges[compU] = {v, weight};
            }

            node = node->next;
        }
    }
}

MST Graph::Boruvka(int16_t numThreads) {
    std::vector<int16_t> parent(size);
    std::vector<int16_t> rank(size, 0);
    std::vector<Node> minEdges(size, {-1, INT16_MAX});
    MST mst; // Для хранения рёбер остовного дерева
    int64_t totalWeight = 0; // Суммарный вес рёбер остовного дерева

    for (int16_t v = 0; v < size; v++) {
        parent[v] = v;
    }

    std::atomic<int16_t> numComponents = size;
    std::vector<std::thread> threads;

    while (numComponents > 1) {
        fill(minEdges.begin(), minEdges.end(), Node(-1, INT16_MAX));

        int16_t step = (size + numThreads - 1) / numThreads;

        for (int16_t t = 0; t < numThreads; t++) {
            int16_t start = t * step;
            int16_t end = std::min((int16_t)(start + step), size);
            threads.emplace_back(&Graph::boruvkaStep, this, std::ref(parent), std::ref(minEdges), start, end);
        }

        for (auto& th : threads) {
            th.join();
        }
        threads.clear();

        for (int16_t u = 0; u < size; ++u) {
            if (minEdges[u].endVertex != -1) {
                int16_t v = minEdges[u].endVertex;
                int16_t weight = minEdges[u].weight;

                int16_t compU = Find(u, parent);
                int16_t compV = Find(v, parent);

                if (compU != compV) {
                    mst.emplace_back(u, v, weight);
                    totalWeight += weight;
                    Union(compU, compV, parent, rank);
                    numComponents--;
                }
            }
        }
    }

    return mst;
}