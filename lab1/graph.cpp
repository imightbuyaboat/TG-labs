#include "graph.h"

Graph::Graph(const char* fileName, const bool addNewVertex) {
    //открываем бинарный файл для чтения
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    //считываем количество вершин
    file.read(reinterpret_cast<char*>(&size), sizeof(int16_t));
    if(addNewVertex) size++;
    else std::cout << "Size of matrix: " << size << " x " << size << std::endl;

    //выделяем память под матрицу смежности
    adjaencyMatrix = new int16_t*[size];
    for(size_t i = 0; i < size; i++) {
        adjaencyMatrix[i] = new int16_t[size];
    }

    //заполняем матрицу смежности из файла
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if(addNewVertex && (i == size - 1 || j == size - 1)) {
                adjaencyMatrix[i][j] = INT16_MAX;
            }
            else {
                int16_t value;
                file.read(reinterpret_cast<char*>(&value), sizeof(int16_t));
                if(value == 0) value = INT16_MAX;
                adjaencyMatrix[i][j] = value;
            }
        }
    }

    //заполняем новый столбец расширенного графа для алгоритма Джонсона
    if(addNewVertex) {
        for(size_t i = 0; i < size; i++) adjaencyMatrix[size - 1][i] = 0;
    }

    file.close();
}

Graph::~Graph() {
    for(size_t i = 0; i < size; i++) {
        delete[] adjaencyMatrix[i];
    }
    delete[] adjaencyMatrix;
}

int16_t* Graph::BellmanFord(const int16_t src) {
    //создаем и заполняем массив длин кратчайших рутей
    int16_t* dist = new int16_t[size];
    for(size_t i = 0; i < size; i++) {
        dist[i] = INT16_MAX;
    }
    dist[src] = 0;

    //создание и заполнение контейнера set множеством ребер
    std::set<vertexPair> vertexPairs;
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if(adjaencyMatrix[i][j] != INT16_MAX && i != j) {
                vertexPairs.insert({i, j});
            }
        }
    }

    //проходим по всем ребрам (size-1) раз и пытаемся улучшить расстояния
    for(size_t i = 0; i < size - 1; i++) {
        for(vertexPair pair : vertexPairs) {
            if(dist[pair.first] != INT16_MAX &&
                dist[pair.second] > dist[pair.first] + adjaencyMatrix[pair.first][pair.second]) {
                dist[pair.second] = dist[pair.first] + adjaencyMatrix[pair.first][pair.second];
            }
        }
    }

    //определяем наличие отрицательного цикла
    bool negativeCycle = false;
    for(vertexPair pair : vertexPairs) {
        if(dist[pair.first] != INT16_MAX &&
            dist[pair.second] > dist[pair.first] + adjaencyMatrix[pair.first][pair.second]) {
            negativeCycle = true;
            break;
        }
    }

    if(negativeCycle) return nullptr;
    return dist;
}

void Graph::Dijkstra(const int16_t src, int16_t* distance) {
    //инициализация массива расстояний
    for(size_t i = 0; i < size; i++) distance[i] = INT16_MAX;
    distance[src] = 0;

    //создаем очередь
    std::priority_queue<vertexPair, std::vector<vertexPair>, std::greater<vertexPair>> pq;
    pq.push({0, src});

    //пока очередь не пустая
    while (!pq.empty()) {
        //извлекаем вершину u с наименьшим расстоянием
        int16_t u = pq.top().second;
        pq.pop();

        //цикл по всем ребрам (u, v) 
        for (int16_t v = 0; v < size; v++) {
            if (adjaencyMatrix[u][v] != INT16_MAX) {
                //считаем альтернативное расстояние и сравниваем с текущим расстоянием
                int16_t alt = distance[u] + adjaencyMatrix[u][v];
                if (alt < distance[v]) {
                    distance[v] = alt;
                    pq.push({distance[v], v});
                }
            }
        }
    }
}

int16_t** Graph::Johnson(const char* fileName) {
    //создаем расширенный граф
    Graph expandedGraph(fileName, true);

    int16_t* h = expandedGraph.BellmanFord(size);
    if(!h) return nullptr;

    //корректируем все ребра для устранения ребер с отрицательным весом
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++) {
            if (expandedGraph.getElement(i, j) != INT16_MAX) {
                expandedGraph.getElement(i, j) += h[i] - h[j];
            }
        }
    }

    //матрица кратчайших расстояний
    int16_t** allPairsShortestPaths = new int16_t*[size];
    int16_t* distance = new int16_t[size + 1];  //массив для кратчайших расстояний

    //цикл для каждой вершиный графа
    for (size_t i = 0; i < size; i++) {
        expandedGraph.Dijkstra(i, distance);

        allPairsShortestPaths[i] = new int16_t[size];

        //восстанавливаем истинные расстояния в графе
        for (size_t j = 0; j < size; j++) {
            if (distance[j] != INT16_MAX) {
                allPairsShortestPaths[i][j] = distance[j] + h[j] - h[i];
            } else {
                allPairsShortestPaths[i][j] = INT16_MAX;
            }
        }
    }

    delete[] h;
    delete[] distance;

    return allPairsShortestPaths;
}