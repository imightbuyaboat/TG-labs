#include "graph.h"

Graph::Graph(const char *fileName)
{
    // открываем бинарный файл для чтения
    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    // считваем количество вершин
    file.read(reinterpret_cast<char *>(&width), sizeof(int16_t));
    file.read(reinterpret_cast<char *>(&height), sizeof(int16_t));

    // выделяем память под матрицу
    matrix = new int16_t *[height];
    for (size_t i = 0; i < height; i++)
    {
        matrix[i] = new int16_t[width];
    }

    // заполняем матрицу смежности из файла
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            file.read(reinterpret_cast<char *>(&matrix[i][j]), sizeof(int16_t));
        }
    }

    file.close();
}

Graph::~Graph()
{
    for (int16_t i = 0; i < height; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

A_Result Graph::A(Node start, Node end, int16_t (*h)(int16_t, int16_t, int16_t, int16_t), bool isNeedToCheckDiagonally)
{
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;

    start.h = h(start.x, start.y, end.x, end.y);
    openList.push(start);

    size_t totalNodes = height * width, visitedNodes = 0;

    std::list<Node> path;

    std::vector<std::pair<int16_t, int16_t>> directions;
    if(isNeedToCheckDiagonally) {
        directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    }
    else directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; 

    // Основной цикл алгоритма A*
    while (!openList.empty())
    {
        Node current = openList.top();
        openList.pop();

        // Если узел уже посещен, пропустить
        if (visited[current.x][current.y]) continue;

        // Пометить узел как посещенный и увеличить счетчик посещений
        visited[current.x][current.y] = true;
        visitedNodes++;

        // Проверка, достигли ли мы целевого узла
        if (current.x == end.x && current.y == end.y)
        {
            double pathLength = current.g;

            // Восстановление пути из цепочки родителей
            while (current.parent)
            {
                path.push_front(current);
                current = *current.parent;
            }
            path.push_front(start);

            //std::cout<<visitedNodes<<std::endl;
 
            return A_Result(path, visitedNodes); // Возвращаем найденный путь
        }

        // Проверка соседних узлов
        for (const auto &dir : directions)
        {
            int nx = current.x + dir.first;
            int ny = current.y + dir.second;

            // Проверка границ и того, что узел не был посещен
            if (nx >= 0 && nx < height && ny >= 0 && ny < width && !visited[nx][ny])
            {
                // Рассчет стоимости перехода на соседний узел
                double new_g = current.g + abs(matrix[nx][ny] - matrix[current.x][current.y]) + 1;
                double new_h = h(nx, ny, end.x, end.y);

                // Создание нового узла-соседа с обновленными значениями
                Node neighbor(nx, ny, new_g, new_h, new Node(current));

                // Добавление узла в очередь на проверку
                openList.push(neighbor);
            }
        }
    }

    return A_Result({}, -1);
}

int16_t euclideanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend)
{
    return static_cast<int16_t>(std::sqrt(std::pow(xend - x, 2) + std::pow(yend - y, 2)));
}

int16_t manhattanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend)
{
    return std::abs(xend - x) + std::abs(yend - y);
}

int16_t chebyshevHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend)
{
    return std::max(std::abs(xend - x), std::abs(yend - y));
}