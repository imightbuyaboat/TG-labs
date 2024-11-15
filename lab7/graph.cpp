#include "graph.h"

Graph::Graph(const char *fileName) {
    // открываем бинарный файл для чтения
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    // считываем количество вершин
    file.read(reinterpret_cast<char *>(&size), sizeof(int16_t));
    std::cout << "Size of matrix: " << size << " x " << size << std::endl;

    // выделяем память под матрицу смежности
    adjaencyMatrix = new int16_t *[size];
    for (size_t i = 0; i < size; i++) {
        adjaencyMatrix[i] = new int16_t[size];
    }

    // заполняем матрицу смежности из файла
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            file.read(reinterpret_cast<char *>(&adjaencyMatrix[i][j]), sizeof(int16_t));
        }
    }

    file.close();
}

Graph::~Graph() {
    for (size_t i = 0; i < size; i++) {
        delete[] adjaencyMatrix[i];
    }
    delete[] adjaencyMatrix;
}

//функция обхода графа в ширину для выделения компоненты связности
void Graph::bfs(size_t to, std::vector<bool> &visited, std::vector<size_t> &component) {
    std::queue<size_t> q;

    //начинаем обход с вершины `to`
    q.push(to);
    visited[to] = true;
    component.push_back(to);

    //пока очередь не пустая
    while (!q.empty()) {
        size_t from = q.front();
        q.pop();

        //обходим всех соседей вершины `from`
        for (size_t w = 0; w < size; ++w) {
            if (adjaencyMatrix[from][w] == 1 && !visited[w]) {
                visited[w] = true;

                //добавляем вершину в очередь
                q.push(w);
                component.push_back(w);
            }
        }
    }
}

//выделяет и возвращает двусвязную компоненту из стека рёбер
std::vector<size_t> Graph::findBiconn(std::list<size_t> &stack, size_t from, size_t to) {
    //множество для хранения уникальных вершин
    std::set<size_t> uniqueVert;
    std::vector<size_t> biconnComp;

    //извлекаем рёбра из стека, пока не найдём указанное ребро
    while (!stack.empty() && stack.back() != from * size + to) {
        size_t edge = stack.back();
        stack.pop_back();

        //добавляем вершины текущего ребра в множество
        uniqueVert.insert(edge / size);
        uniqueVert.insert(edge % size);
    }

    //добавляем последнее ребро, если оно существует
    if (!stack.empty()) {
        size_t edge = stack.back();
        stack.pop_back();
        uniqueVert.insert(edge / size);
        uniqueVert.insert(edge % size);
    }

    //переносим уникальные вершины в вектор двусвязной компоненты
    for (size_t v : uniqueVert)
        biconnComp.push_back(v);

    return biconnComp;
}

void Graph::dfs(size_t from, size_t parent, std::vector<bool> &visited, std::vector<size_t> &disc,
                std::vector<size_t> &low, std::vector<size_t> &parentArr, std::set<std::pair<size_t, size_t>> &bridges,
                std::set<size_t> &aps, std::vector<std::vector<size_t>> &biconnComp, std::list<size_t> &stack) {

    //время посещения вершины                
    static int time = 0;

    visited[from] = true;
    disc[from] = low[from] = ++time;

    //счётчик дочерних вершин
    size_t children = 0;

    //обходим всех соседей вершины `from`
    for (size_t to = 0; to < size; ++to) {
        if (adjaencyMatrix[from][to] == 1) {
            //если вершина ещё не посещена
            if (!visited[to]) {
                children++;

                //устанавливаем родительскую вершину
                parentArr[to] = from;
                stack.push_back(from * size + to);

                //рекурсивный вызов DFS для вершины `to`
                dfs(to, from, visited, disc, low, parentArr, bridges,
                    aps, biconnComp, stack);

                //обновляем низкую точку для `from`
                low[from] = std::min(low[from], low[to]);

                //проверяем, является ли вершина точкой сочленения
                if (parentArr[from] == -1 && children > 1)
                    aps.insert(from);

                if (parentArr[from] != -1 && low[to] >= disc[from])
                    aps.insert(from);

                //если найден мост
                if (low[to] > disc[from])
                    bridges.insert({from, to});

                //если завершена двусвязная компонента
                if (low[to] >= disc[from])
                    biconnComp.push_back(std::move(findBiconn(stack, from, to)));

            }
            //если вершина посещена
            else if (to != parent && disc[to] < disc[from]) {
                //обновляем низкую точку, если нашли обратное ребро
                low[from] = std::min(low[from], disc[to]);

                //добавляем обратное ребро в стек
                if (disc[from] > disc[to])
                    stack.push_back(from * size + to);
            }
        }
    }
}

//функция нахождения всех компонент связности графа
std::vector<std::vector<size_t>> Graph::findConnectedComponents() {
    std::vector<bool> visited(size, false);
    std::vector<std::vector<size_t>> connComp;

    for (size_t to = 0; to < size; to++) {
        //если вершина ещё не посещена
        if (!visited[to]) {
            std::vector<size_t> component;

            //находим компоненту с помощью BFS
            bfs(to, visited, component);
            std::sort(component.begin(), component.end());
            connComp.push_back(component);
        }
    }

    return connComp;
}

//функция нахождения мостов, точек сочленения и двусвязных компонент графа
void Graph::findBridgeAps(std::set<std::pair<size_t, size_t>>& bridges,
                            std::set<size_t>& aps,
                            std::vector<std::vector<size_t>>& biconnComp) {
                                
    std::vector<bool> visited(size, false);
    std::vector<size_t> disc(size, -1), low(size, -1), parentArr(size, -1);
    std::list<size_t> stack;

    for (size_t to = 0; to < size; to++)
        if (!visited[to])
            dfs(to, -1, visited, disc, low, parentArr, bridges,
                aps, biconnComp, stack);
}