#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "linkedlist.h"

struct Bucket {
    LinkedList list; 
};

class HashTable {
// csvreader is a friend to be able to access the hash function
friend class CSVReader;
private:
    Bucket* buckets; // pointer to an array of Bucket
    size_t hash(const std::string& key) const; // hash function
    static const size_t defaultSize = 2053; // default number of buckets (prime number)

public:
    size_t numBuckets; // number of buckets
    HashTable(size_t numBuckets = defaultSize);
    ~HashTable();
    void insert(const std::string& domain, const std::string& username="", const std::string& password=""); // inserts key into a new node, with key as the first string in the node's array
    bool find(const std::string& key) const; // finds a node that contains the key in the first position of its string array
    void remove(const std::string& domain, const std::string& username); // removes the node that contains the key in the first position of its string array
    void insertAtIndex(size_t index, const std::string& domain, const std::string& username, const std::string& password);
    bool update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername=""); // update entry by hash
    Bucket* getBuckets() const;
    Bucket* getBucket(const std::string& domain) const;
    size_t hashNoMod(const std::string& key) const; // hash function without modulo 
};

#endif // HASHTABLE_H
