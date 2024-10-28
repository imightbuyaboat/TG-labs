#include <string.h>
#include "graph.h"

char* outputFileName = "default.txt";

void writeResultToFile(std::ofstream& file, PathResult* result, const char* Heuristic) {
    if(result) {
        file << Heuristic << ":" << std::endl;
        file << "Длина пути: " << result->totalCost << std::endl
            << "Процент просмотренных клеток: " << result->percentVisited << "%" << std::endl;
        // for(auto& vertex : result->path) {
        //     file << "[" << vertex.x << ", " << vertex.y << "] ";
        // }
        // file << std::endl << std::endl;
        file << std::endl;

        delete result;
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


    std::ofstream file(outputFileName);
    if (!file) {
        std::cerr << "Can not open file: " << outputFileName << std::endl;
        return 1;
    }

    Graph graph(argv[1]);

    PathResult* result = graph.A(xstart, ystart, xend, yend, chebyshevHeuristic, false);
    writeResultToFile(file, result, "Чебышев");
    
    result = graph.A(xstart, ystart, xend, yend, euclideanHeuristic, false);
    writeResultToFile(file, result, "Евклид");

    result = graph.A(xstart, ystart, xend, yend, euclideanHeuristic, false);
    writeResultToFile(file, result, "Манхэттен");

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}