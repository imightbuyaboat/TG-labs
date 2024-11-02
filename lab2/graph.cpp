#include "graph.h"

//конструктор
Graph::Graph(const char* fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    //считваем количество вершин
    file.read(reinterpret_cast<char*>(&size), sizeof(int16_t));
    std::cout << "size: " << size << std::endl;

    //создаем список смежности
    adjacencyList = new Node*[size];
    for(int16_t i = 0; i < size; i++) adjacencyList[i] = nullptr;

    //считваем ребра и добавляем в список
    while(true) {
        int16_t startVertex, endVertex, weight;
        file.read(reinterpret_cast<char*>(&startVertex), sizeof(int16_t));

        if (file.eof()) break;

        file.read(reinterpret_cast<char*>(&endVertex), sizeof(int16_t));
        file.read(reinterpret_cast<char*>(&weight), sizeof(int16_t));

        if(weight == 0) continue;

        AddEdge(startVertex, endVertex, weight);
    }

    //создаем матрицу потоков с нулевыми значениями для каждого ребра
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

//функция добавлениЯ ребра в список смежности
void Graph::AddEdge(int16_t src, int16_t end, int16_t weight) {
    //добавляем ребро в список
    Node* node = new Node(end, weight);
    node->next = adjacencyList[src];
    adjacencyList[src] = node;

    //проверяем существует ли обратное ребро в списке
    Node* temp = adjacencyList[end];
    bool reverseEdgeExists = false;
    while (temp) {
        if (temp->endVertex == src) {
            reverseEdgeExists = true;
            break;
        }
        temp = temp->next;
    }

    //если не существует, то добавляем его в список
    if (!reverseEdgeExists) {
        node = new Node(src, 0);
        node->next = adjacencyList[end];
        adjacencyList[end] = node;
    }
}

// bool Graph::DFS(int16_t src, int16_t end, bool* visited, int16_t* parent) {
//     visited[src] = true;

//     if(src == end) return true;

//     Node* temp = adjacencyList[src];
//     while(temp) {
//         if(temp->weight == 0) {
//             temp = temp->next;
//             continue;
//         }

//         int16_t u = temp->endVertex;
//         int16_t currCapacity = temp->weight - flow[src][u];

//         if(!visited[u] && currCapacity > 0) {
//             parent[u] = src;
//             if(DFS(u, end, visited, parent)) return true;
//         }
//         temp = temp->next;
//     }
//     return false;
// }

//метод BFS для поиска пути с положительной пропускной способностью
bool Graph::BFS(int16_t src, int16_t end, bool* visited, int16_t* parent) {
    std::fill(visited, visited + size, false);

    //создаем очередь и добавляем начальную вершину
    std::queue<int16_t> q;
    q.push(src);

    visited[src] = true;

    //выполняем BFS
    while (!q.empty()) {
        //достаем очередную вершину из очереди
        int16_t u = q.front();
        q.pop();

        Node* temp = adjacencyList[u];
        while (temp) {
            //пропускаем ребра с нулевым весом
            if (temp->weight == 0) {
                temp = temp->next;
                continue;
            }

            int16_t v = temp->endVertex;    //вершина
            int16_t currCapacity = temp->weight - flow[u][v];   //текущая пропускная способность ребра (u, v)

            //если вершина не посещена и остаточная пропускная способность положительная
            if (!visited[v] && currCapacity > 0) {
                parent[v] = u;
                visited[v] = true;

                //если достигнута конечная вершина
                if (v == end) return true;

                //добавляем очередную вершину в очередь
                q.push(v);
            }
            temp = temp->next;
        }
    }
    return false;
}

long Graph::FordFalkenson(int16_t src, int16_t end) {
    bool* visited = new bool[size];         //массив посещенных вершин
    int16_t* parent = new int16_t[size];    //массив для восстановления пути
    long maxFlow = 0;      //максимальный поток

    while(true) {
        std::fill(visited, visited + size, false);
        std::fill(parent, parent + size, -1);

        //ищем путь с положительной пропускной способностью
        if(!BFS(src, end, visited, parent)) break;

        //находим минимальную пропускную способность на пути
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

        //обновляем потоки вдоль найденного пути
        for(int16_t v = end; v != src; v = parent[v]) {
            int16_t u = parent[v];
            flow[u][v] += pathFlow;
            flow[v][u] -= pathFlow;
        }

        maxFlow += pathFlow;
    }

    std::cout << "MaxFlow: " << maxFlow << std::endl;

    delete[] visited, parent;
    return maxFlow;
}

int16_t** Graph::GetFlow() { return flow; }
Node** Graph::GetAdjacencyList() { return adjacencyList; }
int16_t Graph::GetSize() { return size; }