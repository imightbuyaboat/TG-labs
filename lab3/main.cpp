#include <string.h>
#include <chrono>
#include "graph.h"

char* outputFileName = "default.txt";

void writeResultToFile(std::ofstream& file, std::list<Node> path, size_t visitedNodes, const char* Heuristic, std::pair<int16_t, int16_t> sizes, std::chrono::duration<double> duration) {
    if(!path.empty()) {
        file << Heuristic << ":" << std::endl;

        const auto &end = path.back();
        file << "Длина пути: " << end.g << std::endl;

        file << "Процент просмотренных клеток: " << 
            100.0 * visitedNodes / (sizes.first * sizes.second) << "%" << std::endl;

        file << "Время выполнения: " << duration.count() << std::endl;

        for(const auto &node : path) {
            file << "[" << node.x << ", " << node.y << "] ";
        }
        file << std::endl << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 8 && argc != 10) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    //меняем название выходного файла
    if (argc == 10) {
        for(int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0 && i != argc - 1) {
                outputFileName = new char[strlen(argv[i + 1]) + 1];
                strcpy(outputFileName, argv[3]);
                break;
            }
        }
    }

    int16_t xstart, ystart, xend, yend;
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && argc > i + 2) {
            xstart = static_cast<int16_t>(std::stoi(argv[i+1]));
            ystart = static_cast<int16_t>(std::stoi(argv[i+2]));
        }
        if (strcmp(argv[i], "-e") == 0 && argc > i + 2) {
            xend = static_cast<int16_t>(std::stoi(argv[i+1]));
            yend = static_cast<int16_t>(std::stoi(argv[i+2]));
        }
    }
    Node start(xstart, ystart), end(xend, yend);

    std::ofstream file(outputFileName);
    if (!file) {
        std::cerr << "Can not open file: " << outputFileName << std::endl;
        return 1;
    }

    Graph graph(argv[1]);
    auto sizes = graph.getSizes();
    size_t visitedNodes = 0;

    std::chrono::system_clock::time_point startTime, endTime;
    std::chrono::duration<double> duration;

    file << "length of path between (" << xstart << ", " << ystart << ") and (" <<
        xend << ", " << yend << ") points" << std::endl << std::endl;
    
    startTime = std::chrono::high_resolution_clock::now();
    auto path = graph.A(start, end, chebyshevHeuristic, true, visitedNodes);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, path, visitedNodes, "Чебышев", sizes, duration);
    
    visitedNodes = 0;
    startTime = std::chrono::high_resolution_clock::now();
    path = graph.A(start, end, euclideanHeuristic, true, visitedNodes);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, path, visitedNodes, "Евклид", sizes, duration);

    visitedNodes = 0;
    startTime = std::chrono::high_resolution_clock::now();
    path = graph.A(start, end, euclideanHeuristic, false, visitedNodes);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, path, visitedNodes, "Манхэттен", sizes, duration);

    visitedNodes = 0;
    startTime = std::chrono::high_resolution_clock::now();
    path = graph.A(start, end, [](int16_t, int16_t, int16_t, int16_t)->int16_t {return 0;}, false, visitedNodes);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, path, visitedNodes, "Дейкстра", sizes, duration);

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}