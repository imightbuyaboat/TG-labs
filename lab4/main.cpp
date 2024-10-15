#include <iostream>
#include <cstring>
#include <chrono>
#include "tree.h"

#define MIN_ELEMENTS_COUNT 10

void menu() {
    std::cout << "\n1. Search element\n" <<
                "2. Insert element\n" <<
                "3. Print 10 minimal elements\n" <<
                "Print number: ";
}

int32_t input(int32_t min, int32_t max) {
    int32_t value;
    while (!(std::cin >> value) || value < min || value > max) {
        std::cout << "Print again: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    return value;
}

void createTree(const int32_t size, const char* fileName) {
    std::ofstream file(fileName, std::ios::binary);

    file.write(reinterpret_cast<const char*>(&size), sizeof(int32_t));

    for (int32_t i = 0; i < size; i++) {
        //int32_t value = rand() % (INT32_MAX) - INT32_MAX / 2;

        int32_t value = rand() % (100) - 100 / 2;
        file.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    }

    file.close();
}

int main(const int argc, const char* argv[]) {
    if(argc != 2 && argc != 4) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        int32_t size = static_cast<int32_t>(std::stoi(argv[3]));
        createTree(size, argv[2]);
        return 0;
    }

    Tree tree;
    tree.InitializeFromFile(argv[1]);
    
    std::chrono::system_clock::time_point start, end;
    int32_t value;
    bool found, added;

    while(true) {
        tree.Print();
        menu();

        switch(input(1, 3)) {
        case 1:
            std::cout << "Print value: ";
            value = input(INT32_MIN, INT32_MAX);

            start = std::chrono::high_resolution_clock::now();
            found = tree.Search(value);
            end = std::chrono::high_resolution_clock::now();

            if(found) std::cout << "Element " << value << " is found\n";
            else std::cout << "Element " << value << " isn`t found\n";
            break;
        
        case 2:
            std::cout << "Print value: ";
            value = input(INT32_MIN, INT32_MAX);

            start = std::chrono::high_resolution_clock::now();
            added = tree.Insert(value);
            end = std::chrono::high_resolution_clock::now();

            if(added) std::cout << "Element " << value << " is found\n";
            else std::cout << "Element " << value << " isn`t found\n";
            break;

        case 3:
            start = std::chrono::high_resolution_clock::now();
            auto min = tree.SearchXMin(MIN_ELEMENTS_COUNT);
            end = std::chrono::high_resolution_clock::now();

            if(!min) std::cout << "Tree has less than " << MIN_ELEMENTS_COUNT << " elements\n";
            else {
                std::cout << "[";
                for(size_t i = 0; i < MIN_ELEMENTS_COUNT; i++) {
                    std::cout << min[i];
                    if (i != MIN_ELEMENTS_COUNT - 1) std::cout << ", ";
                }
                std::cout << "]\n";

                delete[] min;
            }
            break;
        }

        std::chrono::duration<double> duration = end - start;
        std::cout << "Execution time: " << std::fixed << std::setprecision(6) 
            << duration.count() << " sec\n" << std::endl;
    }
}