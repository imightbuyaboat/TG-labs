#include "graph.h"

Graph::Graph(const char* fileName) {
    //открываем бинарный файл для чтения
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Can not open file: " << fileName << std::endl;
        exit(1);
    }

    //считваем количество вершин
    file.read(reinterpret_cast<char*>(&width), sizeof(int16_t));
    file.read(reinterpret_cast<char*>(&height), sizeof(int16_t));

    //выделяем память под матрицу
    matrix = new int16_t*[height];
    for(size_t i = 0; i < height; i++) {
        matrix[i] = new int16_t[width];
    }

    //заполняем матрицу смежности из файла
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            file.read(reinterpret_cast<char*>(&matrix[i][j]), sizeof(int16_t));
        }
    }

    file.close();
}

Graph::~Graph() {
    for (int16_t i = 0; i < height; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

PathResult* Graph::A(int16_t xstart, int16_t ystart, int16_t xend, int16_t yend, int16_t (*h)(int16_t, int16_t, int16_t, int16_t), bool isNeedToCheckDiagonally) {
    int16_t** g = new int16_t*[height], **f = new int16_t*[height];
    for(int16_t i = 0; i < height; i++) {
        g[i] = new int16_t[width];
        f[i] = new int16_t[width];
        std::fill(g[i], g[i] + width, INT16_MAX);
        std::fill(f[i], f[i] + width, INT16_MAX);
    }
    g[xstart][ystart] = 0;
    f[xstart][ystart] = h(xstart, ystart, xend, yend);

    Cell** cameFrom = new Cell*[height * width];
    std::fill(cameFrom, cameFrom + height * width, nullptr);

    std::priority_queue<Cell, std::vector<Cell>> openSet;
    openSet.push(Cell(xstart, ystart, 0));

    bool pathFound = false;
    int visitedCells = 0;

    while (!openSet.empty()) {
        Cell current = openSet.top();
        openSet.pop();

        visitedCells++;

        // Проверка достижения целевой точки
        if (current.x == xend && current.y == yend) {
            pathFound = true;
            break;
        }

        std::vector<std::pair<int16_t, int16_t>> directions;
        if(isNeedToCheckDiagonally) directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
        else directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        for (const auto& [dx, dy] : directions) {
            int16_t nx = current.x + dx;
            int16_t ny = current.y + dy;
            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                int16_t height_diff = std::abs(matrix[current.x][current.y] - matrix[nx][ny]);
                int16_t tentative_g = g[current.x][current.y] + 1;

                if (tentative_g < g[nx][ny]) {
                    cameFrom[nx * width + ny] = new Cell(current.x, current.y, f[current.x][current.y]);
                    g[nx][ny] = tentative_g;
                    f[nx][ny] = tentative_g + h(nx, ny, xend, yend);
                    openSet.push(Cell(nx, ny, f[nx][ny]));
                }
            }
        }
    }

    PathResult* result = nullptr;

    // Восстановление пути, если найден
    if (pathFound) {
        result = new PathResult;
        result->totalCost = g[xend][yend];

        std::vector<Cell> path;
        int16_t cx = xend;
        int16_t cy = yend;
        while (cx != xstart || cy != ystart) {
            path.push_back(Cell(cx, cy, f[cx][cy]));
            Cell* prev = cameFrom[cx * width + cy];
            cx = prev->x;
            cy = prev->y;
        }
        path.push_back(Cell(xstart, ystart, f[xstart][ystart]));

        result->path = path;
        result->percentVisited = (static_cast<float>(visitedCells) / (height * width)) * 100;
    }

    // Освобождение памяти
    for (int16_t i = 0; i < height; i++) {
        delete[] g[i];
        delete[] f[i];
    }
    delete[] g;
    delete[] f;

    for (int i = 0; i < height * width; i++) {
        if(cameFrom[i]) delete cameFrom[i];
    }
    delete[] cameFrom;

    return result;
}

int16_t euclideanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend) {
    return static_cast<int16_t>(std::sqrt(std::pow(xend - x, 2) + std::pow(yend - y, 2)));
}

int16_t manhattanHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend) {
    return std::abs(xend - x) + std::abs(yend - y);
}

int16_t chebyshevHeuristic(int16_t x, int16_t y, int16_t xend, int16_t yend) {
    return std::max(std::abs(xend - x), std::abs(yend - y));
}