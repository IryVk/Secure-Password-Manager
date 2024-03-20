#include "csvreader.h"
#include <iostream>

int main() {
    try {
        CSVReader reader("docs/data.csv");
        auto data = reader.readData();

        for (const auto &row : data) {
            for (const auto &cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}