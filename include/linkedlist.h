// check if alreadu compiled
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>

struct Node {
    std::string data[3]; // domain, username, password
    Node* next; // pointer to the next node
    Node(const std::string& data1, const std::string& data2, const std::string& data3)
        : data{data1, data2, data3}, next(nullptr) {} // constructor
};

class LinkedList {
    Node* head; // pointer to the head of the linked list
public:
    LinkedList(); // constructor
    ~LinkedList(); // destructor
    void add(const std::string& data1, const std::string& data2, const std::string& data3); // add a new node
    void print() const; // print the linked list
    bool isEmpty() const; // check if the linked list is empty
    int size() const; // get the size of the linked list
    bool find(const std::string& key) const; // find a node
    bool remove(const std::string& domain, const std::string& username); // remove a node
    bool update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername=""); // update a node
    Node *getHead() const; // get the head of the linked list
    Node *getNode(const std::string& domain, const std::string& user) const; // get a node
};

#endif // LINKEDLIST_H
