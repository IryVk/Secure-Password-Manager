#include "hashtable.h"
#include <fstream>
#include <sstream>
#include <string>

class CSVReader {
private:
    HashTable& hashtable; // reference to the hashtable
    std::string filename; // filename of the CSV file

public:
    CSVReader(HashTable& hashtable, const std::string& filename) : hashtable(hashtable), filename(filename) {} // constructor

    void load(); // load the CSV file
    void save(); // save the CSV file
    void addRow(const std::string& domain, const std::string& username, const std::string& password); // add a new row
    bool updateRow(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername=""); // update a row
    void deleteRow(const std::string& domain, const std::string& username); // delete a row
};
