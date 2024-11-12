#include "graph.h"

Graph::Graph(const char *fileName)
{
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

Graph::~Graph()
{
    for (size_t i = 0; i < size; i++) {
        delete[] adjaencyMatrix[i];
    }
    delete[] adjaencyMatrix;
}

void Graph::bfs(size_t to, std::vector<bool> &visited, std::vector<size_t> &component)
{
    std::queue<size_t> q;
    q.push(to);
    visited[to] = true;
    component.push_back(to);

    while (!q.empty()) {
        size_t from = q.front();
        q.pop();

        for (size_t w = 0; w < size; ++w) {
            if (adjaencyMatrix[from][w] == 1 && !visited[w]) {
                visited[w] = true;
                q.push(w);
                component.push_back(w);
            }
        }
    }
}

std::vector<size_t> Graph::find_biconn(std::list<size_t> &stack, size_t from, size_t to)
{
    std::set<size_t> unique_vert;
    std::vector<size_t> biconn_comp;

    while (!stack.empty() && stack.back() != from * size + to) {
        size_t edge = stack.back();
        stack.pop_back();

        // Добавляем уникальные вершины компоненты, используя множество
        unique_vert.insert(edge / size);
        unique_vert.insert(edge % size);
    }

    if (!stack.empty()) {
        size_t edge = stack.back();
        stack.pop_back();

        // Добавляем оставшиеся уникальные вершины
        unique_vert.insert(edge / size);
        unique_vert.insert(edge % size);
    }

    // Переносим уникальные вершины в вектор двусвязной компоненты
    for (size_t v : unique_vert)
        biconn_comp.push_back(v);

    return biconn_comp;
}

void Graph::dfs(size_t from, size_t parent, std::vector<bool> &visited, std::vector<size_t> &disc,
                std::vector<size_t> &low, std::vector<size_t> &parent_arr, std::set<std::pair<size_t, size_t>> &bridges,
                std::set<size_t> &aps, std::vector<std::vector<size_t>> &biconn_comp, std::list<size_t> &stack)
{
    static int time = 0;
    visited[from] = true;
    disc[from] = low[from] = ++time;
    size_t children = 0;

    for (size_t to = 0; to < size; ++to) {
        if (adjaencyMatrix[from][to] == 1) {

            if (!visited[to]) {
                children++;
                parent_arr[to] = from;
                stack.push_back(from * size + to);

                dfs(to, from, visited, disc, low, parent_arr, bridges,
                    aps, biconn_comp, stack);

                low[from] = std::min(low[from], low[to]);

                if (parent_arr[from] == -1 && children > 1)
                    aps.insert(from);

                if (parent_arr[from] != -1 && low[to] >= disc[from])
                    aps.insert(from);

                if (low[to] > disc[from])
                    bridges.insert({from, to});

                if (low[to] >= disc[from])
                    biconn_comp.push_back(std::move(find_biconn(stack, from, to)));

            }
            else if (to != parent && disc[to] < disc[from]) {
                low[from] = std::min(low[from], disc[to]);

                if (disc[from] > disc[to])
                    stack.push_back(from * size + to);
            }
        }
    }
}

std::vector<std::vector<size_t>> Graph::find_connected_components()
{
    std::vector<bool> visited(size, false);
    std::vector<std::vector<size_t>> conn_comp;

    for (size_t to = 0; to < size; to++) {

        if (!visited[to]) {
            std::vector<size_t> component;
            bfs(to, visited, component);
            std::sort(component.begin(), component.end());
            conn_comp.push_back(component);
        }
    }

    return conn_comp;
}

void Graph::find_bridge_aps(std::set<std::pair<size_t, size_t>>& bridges,
                            std::set<size_t>& aps,
                            std::vector<std::vector<size_t>>& biconn_comp)
{
    std::vector<bool> visited(size, false);
    std::vector<size_t> disc(size, -1), low(size, -1), parent_arr(size, -1);
    std::list<size_t> stack;

    for (size_t to = 0; to < size; to++)
        if (!visited[to])
            dfs(to, -1, visited, disc, low, parent_arr, bridges,
                aps, biconn_comp, stack);
}