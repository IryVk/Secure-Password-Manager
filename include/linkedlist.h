// check if alreadu compiled
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

struct Node {
    std::string data[3];
    Node* next;
    Node(const std::string& data1, const std::string& data2, const std::string& data3)
        : data{data1, data2, data3}, next(nullptr) {}
};

class LinkedList {
    Node* head;
public:
    LinkedList();
    ~LinkedList();
    void add(const std::string& data1, const std::string& data2, const std::string& data3);
    void print() const;
    bool isEmpty() const;
    int size() const;
    bool find(const std::string& key) const;
    bool remove(const std::string& key);
    bool update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername = "");
    Node *getHead() const;
    Node *getNode(const std::string& domain, const std::string& user) const;
};

#endif // LINKEDLIST_H
