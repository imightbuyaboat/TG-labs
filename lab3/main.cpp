#include <string.h>
#include "graph.h"

char* outputFileName = "default.txt";

void writeResultToFile(std::ofstream& file, A_Result result, const char* Heuristic, std::pair<int16_t, int16_t> sizes) {
    if(!result.path.empty()) {
        file << Heuristic << ":" << std::endl;

        const auto &end = result.path.back();
        file << "Длина пути: " << end.g << std::endl;

        file << "Процент просмотренных клеток: " << 
            result.visitedNodes * 100.0 / (sizes.first * sizes.second) << "%" << std::endl;

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

    auto result = graph.A(start, end, chebyshevHeuristic, false);
    writeResultToFile(file, result, "Чебышев", sizes);
    
    result = graph.A(start, end, euclideanHeuristic, false);
    writeResultToFile(file, result, "Евклид", sizes);

    result = graph.A(start, end, euclideanHeuristic, false);
    writeResultToFile(file, result, "Манхэттен", sizes);

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}