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

        std::cout << "[" << startVertex << ", " << endVertex << "]: " << weight << std::endl;
    }
    std::cout << std::endl;

    flow = new int16_t*[size];
    for(int16_t i = 0; i < size; i++) {
        flow[i] = new int16_t[size];
        std::fill(flow[i], flow[i] + size, 0);
    }

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
        delete[] flow[i];
    }
    delete[] flow;
    delete[] adjacencyList;
}

void Graph::AddEdge(int16_t src, int16_t end, int16_t weight) {
    Node* node = new Node(end, weight);
    node->next = adjacencyList[src];
    adjacencyList[src] = node;

    Node* temp = adjacencyList[end];
    bool reverseEdgeExists = false;
    while (temp) {
        if (temp->endVertex == src) {
            reverseEdgeExists = true;
            break;
        }
        temp = temp->next;
    }

    if (!reverseEdgeExists) {
        node = new Node(src, 0);
        node->next = adjacencyList[end];
        adjacencyList[end] = node;
    }
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

bool Graph::DFS(int16_t src, int16_t end, bool* visited, int16_t* parent) {
    visited[src] = true;

    if(src == end) return true;

    Node* temp = adjacencyList[src];
    while(temp) {
        if(temp->weight == 0) {
            temp = temp->next;
            continue;
        }

        int16_t u = temp->endVertex;
        int16_t currCapacity = temp->weight - flow[src][u];

        if(!visited[u] && currCapacity > 0) {
            parent[u] = src;
            if(DFS(u, end, visited, parent)) return true;
        }
        temp = temp->next;
    }
    return false;
}

int16_t Graph::FordFalkenson(int16_t src, int16_t end) {
    bool* visited = new bool[size];
    int16_t* parent = new int16_t[size];
    int16_t maxFlow = 0;

    while(true) {
        std::fill(visited, visited + size, false);
        std::fill(parent, parent + size, -1);

        if(!DFS(src, end, visited, parent)) break;

        int16_t pathFlow = INT16_MAX;
        for(int16_t v = end; v != src; v = parent[v]) {
            int16_t u = parent[v];

            Node* temp = adjacencyList[u];
            while(temp) {
                if(temp->endVertex == v && temp->weight != 0) {
                    pathFlow = pathFlow < temp->weight - flow[u][v] ? 
                        pathFlow : temp->weight - flow[u][v];
                    break;
                }
                temp = temp->next;
            }
        }

        std::cout << end << " - ";
        for(int16_t v = end; v != src; v = parent[v]) {
            int16_t u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;

            std::cout << u;
            if(u != src) std::cout << " - ";
        }
        std::cout << ": " << pathFlow << std::endl;

        maxFlow += pathFlow;
    }

    std::cout << "MaxFlow: " << maxFlow << std::endl;

    delete[] visited, parent;
    return maxFlow;
}

int16_t** Graph::GetFlow() { return flow; }
Node** Graph::GetAdjacencyList() { return adjacencyList; }
int16_t Graph::GetSize() { return size; }