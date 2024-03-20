#include "csvreader.h"
#include <fstream>
#include <sstream>

// constructor
CSVReader::CSVReader(const std::string &filename) : m_filename(filename) {}

// read data from CSV file
std::vector<std::vector<std::string>> CSVReader::readData() {
    // a 2d vector to store the data as strings
    std::vector<std::vector<std::string>> data;
    // open the file
    std::ifstream file(m_filename);
    
    // check if the file is open
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // read the file line by line
    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream lineStream(line);
        std::string cell;

        while (getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    // return the data in 2d vector
    return data;
}
