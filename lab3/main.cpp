#include <string.h>
#include <chrono>
#include "graph.h"

char* outputFileName = "default.txt";

void writeResultToFile(std::ofstream& file, A_Result& result, const char* Heuristic, std::pair<int16_t, int16_t> sizes, std::chrono::duration<double> duration) {
    if(!result.path.empty()) {
        file << Heuristic << ":" << std::endl;

        const auto &end = result.path.back();
        file << "Длина пути: " << end.g << std::endl;

        file << "Процент просмотренных клеток: " << 
            100.0 * result.visitedNodes / (sizes.first * sizes.second) << "%" << std::endl;

        file << "Время выполнения: " << duration.count() << std::endl;

        for(const auto &node : result.path) {
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

    std::chrono::system_clock::time_point startTime, endTime;
    std::chrono::duration<double> duration;

    file << "length of path between (" << xstart << ", " << ystart << ") and (" <<
        xend << ", " << yend << ") points" << std::endl << std::endl;

    startTime = std::chrono::high_resolution_clock::now();
    auto result = graph.A(start, end, chebyshevHeuristic, false);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, result, "Чебышев", sizes, duration);
    
    startTime = std::chrono::high_resolution_clock::now();
    result = graph.A(start, end, euclideanHeuristic, false);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, result, "Евклид", sizes, duration);

    startTime = std::chrono::high_resolution_clock::now();
    result = graph.A(start, end, euclideanHeuristic, false);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, result, "Манхэттен", sizes, duration);

    startTime = std::chrono::high_resolution_clock::now();
    result = graph.A(start, end, [](int16_t, int16_t, int16_t, int16_t)->int16_t {return 0;}, false);
    endTime = std::chrono::high_resolution_clock::now();
    duration = endTime - startTime;
    writeResultToFile(file, result, "Дейкстра", sizes, duration);

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}