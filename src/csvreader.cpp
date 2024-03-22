#include "csvreader.h"

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

void CSVReader::addRow(const std::string& domain, const std::string& username, const std::string& password) {
    size_t index = hashtable.hash(domain);
    hashtable.insertAtIndex(index, domain, username, password);
    save(); // reflect changes in the CSV file
}

void CSVReader::updateRow(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername) {
    if (hashtable.update(domain, username, newPassword)) {
        save(); // reflect changes in the CSV file if update was successful
    }
}

void CSVReader::deleteRow(const std::string& domain, const std::string& username) {
    hashtable.remove(domain, username);
    save(); // reflect changes in the CSV file
}
