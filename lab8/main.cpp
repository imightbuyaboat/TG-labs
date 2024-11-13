#include <iostream>
#include <cstring>
#include <chrono>
#include <iomanip>
#include "RBTree.h"

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

int main(const int argc, const char* argv[]) {
    if(argc != 2) {
        std::cerr << "Incorrect number of arguments: " << argc << std::endl;
        return 1;
    }

    RBTree tree;
    tree.InitializeFromFile(argv[1]);
    
    std::chrono::system_clock::time_point start, end;
    int32_t value;
    Node* found;

    while(true) {        
        menu();

        switch(input(1, 3)) {
        case 1:
            std::cout << "Print value: ";
            value = input(INT32_MIN, INT32_MAX);

            start = std::chrono::high_resolution_clock::now();
            found = tree.findValue(value);
            end = std::chrono::high_resolution_clock::now();

            if(found) std::cout << "Element " << value << " is found\n";
            else std::cout << "Element " << value << " isn`t found\n";
            break;
        
        case 2:
            std::cout << "Print value: ";
            value = input(INT32_MIN, INT32_MAX);

            start = std::chrono::high_resolution_clock::now();
            tree.insertValue(value);
            end = std::chrono::high_resolution_clock::now();

            std::cout << "Element " << value << " added\n";
            break;

        case 3:
            start = std::chrono::high_resolution_clock::now();
            auto min = tree.getSmallest10();
            end = std::chrono::high_resolution_clock::now();

            std::cout << "[";
            for(size_t i = 0; i < 10; i++) {
                std::cout << min[i];
                if (i != 9) std::cout << ", ";
            }
            std::cout << "]\n";

            break;
        }

        std::chrono::duration<double> duration = end - start;
        std::cout << "Execution time: " << std::fixed << std::setprecision(6) 
            << duration.count() << " sec\n" << std::endl;
    }
}