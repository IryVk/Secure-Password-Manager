#ifndef CSVREADER_H
#define CSVREADER_H

#include "hashtable.h"
#include <fstream>
#include <sstream>
#include <string>

class CSVReader {
    HashTable hashtable;

public:
    CSVReader();
    void loadCSV(const std::string& filename);
    void updateRow(const std::string& hash, const std::string& domain, const std::string& username, const std::string& password);
    void deleteRow(const std::string& hash);
};

#endif // CSVREADER_H
