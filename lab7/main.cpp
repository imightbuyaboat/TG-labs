#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "graph.h"

char* outputFileName = "default.txt";
const int16_t SRC = 0;

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    //меняем название выходного файла
    if (argc == 4) {
        if (strcmp(argv[2], "-o") == 0) {
            outputFileName = new char[strlen(argv[3]) + 1];
            strcpy(outputFileName, argv[3]);
        }
        else {
            std::cerr << "Wrong flag: " << argv[2] << std::endl;
            return 1;
        }
    }

    std::ofstream file(outputFileName);
    if (!file) {
        std::cerr << "Can not open file: " << outputFileName << std::endl;
        return 1;
    }

    Graph graph(argv[1]);

    auto conn_comp = graph.find_connected_components();

    std::set<std::pair<size_t, size_t>> bridges;
    std::set<size_t> aps;
    std::vector<std::vector<size_t>> biconn_comp;

    graph.find_bridge_aps(bridges, aps, biconn_comp);

    file << "Компоненты связности:\n";
    sort(conn_comp.begin(), conn_comp.end());
    for (auto &component : conn_comp)
    {
        file << '[';
        for (const auto &to : component)
            file << to << (to == component.back() ? "" : ", ");
        file << "]\n";
    }

    file << "Мосты:\n";
    for (const auto &[from, to] : bridges)
        file << "(" << from << ", " << to << ")" << std::endl;

    file << "Точки сочлинения:\n[";
    size_t i = 1;
    for (const auto &ap : aps)
    {
        file << ap << (i == aps.size() ? "" : ", ");
        i++;
    }
    file << "]\n";

    file << "Компоненты двусвязности:\n";
    for (const auto &comp : biconn_comp)
    {
        file << '[';
        for (int to : comp)
            file << to << (to == comp.back() ? "" : ", ");
        file << "]\n";
    }

    file.close();
    if(argc == 4) delete[] outputFileName;

    return 0;
}