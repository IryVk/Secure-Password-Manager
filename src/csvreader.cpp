#include "csvreader.h"

// load() reads the CSV file and populates the hashtable with the data
void CSVReader::load() {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string hash, domain, username, password;
        std::getline(iss, hash, ',');
        std::getline(iss, domain, ',');
        std::getline(iss, username, ',');
        std::getline(iss, password, ',');

        size_t index = std::stoul(hash) % hashtable.numBuckets; 
        hashtable.insertAtIndex(index, domain, username, password);
    }
}

// save() writes the hashtable data to the CSV file
void CSVReader::save() {
    std::ofstream file(filename);
    auto buckets = hashtable.getBuckets();
    for (size_t i = 0; i < hashtable.numBuckets; ++i) {
        auto node = buckets[i].list.getHead();
        while (node) {
            file << hashtable.hashNoMod(node->data[0]) << "," << node->data[0] << "," << node->data[1] << "," << node->data[2] << std::endl;
            node = node->next;
        }
    }
}

// addRow() adds a new row to the CSV file
void CSVReader::addRow(const std::string& domain, const std::string& username, const std::string& password) {
    size_t index = hashtable.hash(domain);
    hashtable.insertAtIndex(index, domain, username, password);
    save(); // reflect changes in the CSV file
}

// updateRow() updates an existing row in the CSV file
void CSVReader::updateRow(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername) {
    if (hashtable.update(domain, username, newPassword)) {
        save(); // reflect changes in the CSV file if update was successful
    }
}

// deleteRow() deletes a row from the CSV file
void CSVReader::deleteRow(const std::string& domain, const std::string& username) {
    hashtable.remove(domain, username);
    save(); // reflect changes in the CSV file
}
