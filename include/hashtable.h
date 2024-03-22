#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "linkedlist.h"

struct Bucket {
    LinkedList list; 
};

class HashTable {
private:
    Bucket* buckets; // pointer to an array of Bucket
    size_t numBuckets;
    size_t hash(const std::string& key) const; // hash function
    static const size_t defaultSize = 101; // default number of buckets - a prime number

public:
    HashTable(size_t numBuckets = defaultSize);
    ~HashTable();
    void insert(const std::string& domain, const std::string& username="", const std::string& password=""); // inserts key into a new node, with key as the first string in the node's array
    bool find(const std::string& key) const; // finds a node that contains the key in the first position of its string array
    void remove(const std::string& key); // removes the node that contains the key in the first position of its string array
    void insertAtIndex(size_t index, const std::string& domain, const std::string& username, const std::string& password);
    bool update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername=""); // update entry by hash
    Bucket* getBuckets() const;
    Bucket* getBucket(const std::string domain) const;
};

#endif // HASHTABLE_H