#include "hashtable.h"
#include <fstream>
#include <sstream>
#include <string>

class CSVReader {
private:
    HashTable& hashtable;
    std::string filename;

public:
    CSVReader(HashTable& hashtable, const std::string& filename) : hashtable(hashtable), filename(filename) {}

    void load();
    void save();
    void addRow(const std::string& domain, const std::string& username, const std::string& password);
    bool updateRow(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername="");
    void deleteRow(const std::string& domain, const std::string& username);
};
