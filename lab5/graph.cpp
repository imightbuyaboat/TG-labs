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

    //считваем и добавлем в список ребра
    while(true) {
        int16_t startVertex, endVertex, weight;
        file.read(reinterpret_cast<char*>(&startVertex), sizeof(int16_t));

        if (file.eof()) break;

        file.read(reinterpret_cast<char*>(&endVertex), sizeof(int16_t));
        file.read(reinterpret_cast<char*>(&weight), sizeof(int16_t));

        AddEdge(startVertex, endVertex, weight);
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
    }
    delete[] adjacencyList;
}

void Graph::AddEdge(int16_t src, int16_t end, int16_t weight) {
    //добавляем ребро в список
    Node* node = new Node(end, weight);
    node->next = adjacencyList[src];
    adjacencyList[src] = node;

    //добавляем обратное ребро в список
    node = new Node(src, weight);
    node->next = adjacencyList[end];
    adjacencyList[end] = node;
}

Node** Graph::GetAdjacencyList() { return adjacencyList; }
int16_t Graph::GetSize() { return size; }

//функция нахождения корня множества для вершин u
int16_t Graph::Find(int16_t u, std::vector<int16_t>& parent) {
    if (parent[u] != u) parent[u] = Find(parent[u], parent);
    return parent[u];
}

//функция объединения множетсв, к которым принадлежат вершины u и v
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

//шаг алгоритма Борувки для поиска минимальных рёбер, исходящих из компонент связности
void Graph::boruvkaStep(std::vector<int16_t>& parent, std::vector<Node>& minEdges, int16_t start, int16_t end) {
    for (int16_t u = start; u < end; ++u) {
        //находим компоненту вершины u
        int16_t compU = Find(u, parent);

        Node* node = adjacencyList[u];
        while(node) {
            int16_t v = node->endVertex;
            int16_t weight = node->weight;

            //находим компоненту вершины v
            int16_t compV = Find(v, parent);

            //если вершины принадлежат разным компонентам и вес ребра меньше текущего минимума
            if (compU != compV && weight < minEdges[compU].weight) {
                //обновляем минимальное ребро для компоненты compU
                minEdges[compU] = {v, weight};
            }

            node = node->next;
        }
    }
}

MST Graph::Boruvka(int16_t numThreads) {
    std::vector<int16_t> parent(size);  //вектор для представления компоненты для каждой вершины
    std::vector<int16_t> rank(size, 0); //вектор для хранения рангов для объединения с ранговой эвристикой
    std::vector<Node> minEdges(size, {-1, INT16_MAX});  //минимальные рёбра для каждой компоненты
    MST mst; //массив для хранения рёбер остовного дерева

    //инициализируем parent так, чтобы каждая вершина была в собственной компоненте
    for (int16_t v = 0; v < size; v++) {
        parent[v] = v;
    }

    //количество компонент связности в графе
    std::atomic<int16_t> numComponents = size;

    //массив потоков
    std::vector<std::thread> threads;

    //пока количество компонент больше одной
    while (numComponents > 1) {
        fill(minEdges.begin(), minEdges.end(), Node(-1, INT16_MAX));

        //разделяем вершины для параллельного выполнения
        int16_t step = (size + numThreads - 1) / numThreads;

        //создаем потоки для параллельного выполнения boruvkaStep
        for (int16_t t = 0; t < numThreads; t++) {
            int16_t start = t * step;
            int16_t end = std::min((int16_t)(start + step), size);
            threads.emplace_back(&Graph::boruvkaStep, this, std::ref(parent), std::ref(minEdges), start, end);
        }

        //ожидаем завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();

        //gроходим по минимальным рёбрам и объединяем компоненты
        for (int16_t u = 0; u < size; ++u) {
            if (minEdges[u].endVertex != -1) {
                int16_t v = minEdges[u].endVertex;
                int16_t weight = minEdges[u].weight;

                int16_t compU = Find(u, parent);
                int16_t compV = Find(v, parent);

                if (compU != compV) {
                    //добавляем ребро в минимальное остовное дерево
                    mst.emplace_back(u, v, weight);

                    //объединфем компоненты
                    Union(compU, compV, parent, rank);
                    numComponents--;
                }
            }
        }
    }

    return mst;
}