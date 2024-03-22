#include "linkedlist.h"

// constructor
LinkedList::LinkedList() : head(nullptr) {}


// destructor
LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// add data to the linked list
void LinkedList::add(const std::string& data1, const std::string& data2, const std::string& data3) {
    Node* newNode = new Node(data1, data2, data3);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// print the linked list
void LinkedList::print() const {
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data[0] << ", " << current->data[1] << ", " << current->data[2] << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

// check if the linked list is empty
bool LinkedList::isEmpty() const {
    return head == nullptr;
}

// get the size of the linked list
int LinkedList::size() const {
    int count = 0;
    Node* current = head;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

// find a key in the linked list
bool LinkedList::find(const std::string& key) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->data[0] == key) {
            return true; // Key found
        }
        current = current->next;
    }
    return false; // Key not found
}

// remove a key from the linked list
bool LinkedList::remove(const std::string& key) {
    Node *current = head, *prev = nullptr;
    while (current != nullptr) {
        if (current->data[0] == key) {
            if (prev == nullptr) {
                // key found at head
                head = current->next;
            } else {
                // key found after head
                prev->next = current->next;
            }
            delete current; // free the memory of the node
            return true; // key was found and removed
        }
        prev = current;
        current = current->next;
    }
    return false; // Key not found
}

#include <algorithm> 
#include <cctype>

// utility function for case-insensitive string comparison
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.length() != b.length()) return false;
    for (size_t i = 0; i < a.length(); ++i) {
        if (std::tolower(a[i]) != std::tolower(b[i])) return false;
    }
    return true;
}

// update a domain, username, and password
bool LinkedList::update(const std::string& domain, const std::string& username, const std::string& newPassword, const std::string& newUsername) {
    Node* current = getNode(domain, username);
    if (current != nullptr) {
        // check if both domain and username match (case-insensitive)
        if (equalsIgnoreCase(current->data[0], domain) && equalsIgnoreCase(current->data[1], username)) {
            if (!newUsername.empty())
                current->data[1] = newUsername; // update username
            current->data[2] = newPassword; // update password
            return true; // successfully updated
        }
    }
    return false; // identifiers not found
}

// get the head of the linked list
Node* LinkedList::getHead() const {
    return head;
}

// get a node with a specific domain and username
Node* LinkedList::getNode(const std::string& domain, const std::string& user) const {
    Node* current = head;
    while (current != nullptr) {
        if (equalsIgnoreCase(current->data[0], domain) && equalsIgnoreCase(current->data[1], user)) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // key not found
}