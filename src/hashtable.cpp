#include "hashtable.h"

// constructor
HashTable::HashTable(size_t numBuckets) : numBuckets(numBuckets) {
    buckets = new Bucket[numBuckets]; // dynamically allocate the array of buckets
}

// destructor
HashTable::~HashTable() {
    delete[] buckets; // clean up the dynamically allocated array
}

// simple hash function
size_t HashTable::hash(const std::string& key) const {
    size_t hashValue = 0;
    for (char c : key) {
        hashValue = 37 * hashValue + c; // 37 is a prime number
    }
    return hashValue % numBuckets;
}

// insert a key into the hash table
void HashTable::insert(const std::string& domain, const std::string& username, const std::string& password) {
    size_t index = hash(domain);
    buckets[index].list.add(domain, username, password); // only the first string is used
}

// find a key in the hash table
bool HashTable::find(const std::string& key) const {
    size_t index = hash(key);
    return buckets[index].list.find(key); // finds based on the first string in each node's array
}

// remove a key from the hash table
void HashTable::remove(const std::string& key) {
    size_t index = hash(key);
    buckets[index].list.remove(key); // removes based on the first string in each node's array
}

// insert a domain, username, and password at a specific index in the hash table
void HashTable::insertAtIndex(size_t index, const std::string& domain, const std::string& username, const std::string& password) {
    if (index >= numBuckets) {
        std::cerr << "Index out of bounds for hashtable insertion: " << index << std::endl;
        return;
    }
    // directly access the bucket at the specified index and add the new node
    buckets[index].list.add(domain, username, password);
}

// update a domain, username, and password based on a hash value
bool HashTable::update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername) {
    size_t index = hash(domain);
    if (index >= numBuckets) return false; // index out of bounds check
    return buckets[index].list.update(domain, username, newPassword, newUsername);
}

// get the array of buckets
Bucket* HashTable::getBuckets() const {
    return buckets;
}

// get a specific bucket at an index
Bucket* HashTable::getBucket(const std::string domain) const {
    size_t index = hash(domain);
    if (index >= numBuckets) return nullptr; // index out of bounds check
    return &buckets[index];
}