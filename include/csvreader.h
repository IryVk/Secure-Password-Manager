#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader(const std::string &filename);
    std::vector<std::vector<std::string>> readData();

private:
    std::string m_filename;
};

#endif // CSVREADER_H
