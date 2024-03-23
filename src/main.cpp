#include "aes-ecb.h"
#include "aes-gcm.h"
#include "csvreader.h"
#include "hashtable.h"
#include "keyivgen.h"
#include "passgenerator.h"
#include "passhash.h"
#include "userhandler.h"
#include <fstream>
#include <limits> 
#include <cstdlib>  
#include <csignal>  
#include <exception>
#include <sstream>
#include <string>
#include <cctype>

using namespace CryptoPP;

// globals to store key, iv and username
std::string KEY = "";
std::string IV = "";
std::string USER = "";

void cleanup();
void clearCin();
int getInt(std::string& prompt);
void displayInitMenu();
bool createUser(std::string& key, std::string& iv, std::string& user);
bool verifyUser(std::string& key, std::string& iv, std::string& user);
void displayMenu2();
bool addPassword(CSVReader& reader, HashTable& ht, std::string& key);
int searchPasswords(CSVReader& reader, HashTable& ht, Bucket** index);
void manipulatePassword(CSVReader& reader, HashTable& ht, Bucket* index, std::string& key);
bool updatePassword(CSVReader& reader, int recordNumber, Bucket* index, std::string& key);
void showPassword(int recordNumber, Bucket* index, std::string& key);
bool deletePassword(CSVReader& reader, int recordNumber, Bucket* index);
void signalHandler(int signal);
void viewPasswords(CSVReader& reader, HashTable& ht);

int main() {
    // clean up unenctypted files
    std::atexit(cleanup);
    // Register the signal handler for SIGINT
    std::signal(SIGINT, signalHandler);

    // display initial menu
    std::cout << "Welcome to the Password Manager!\n";
    // make empty key and iv
    std::string username;
    std::string key;
    std::string iv;
    while (true) {
        displayInitMenu();
        int choice;
        std::string prompt = "Enter your choice: ";
        choice = getInt(prompt);
        switch (choice) {
            case 1:
                if (!createUser(key, iv, username)) {
                    std::exit(1);
                }
                break;
            case 2:
                if (!verifyUser(key, iv, username)) {
                    std::exit(1);
                }
                break;
            case 3:
                std::exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        break;
    }

    // put data into globals
    KEY = key;
    IV = iv;
    USER = username;

    // unencrypt passwords file
    // cast key and iv to SecByteBlock
    SecByteBlock keyblock((byte*)key.data(), key.size());
    SecByteBlock ivblock((byte*)iv.data(), iv.size());
    AESCrypto aesCrypto(keyblock, ivblock);
    aesCrypto.DecryptFile("assets/" + username + ".csv", "assets/" + username + "_decrypted.csv");
    // read passwords file
    HashTable ht;
    CSVReader reader(ht, "assets/" + username + "_decrypted.csv");
    reader.load();

    while (true) {
        // display menu
        displayMenu2();
        int choice;
        std::string prompt = "Enter your choice: ";
        choice = getInt(prompt);
        int n;
        // index to hold bucket
        Bucket* index = nullptr;
        switch (choice) {
            case 1:
                if (!addPassword(reader, ht, key)) {
                    std::exit(1);
                }
                break;
            case 2:
                n = searchPasswords(reader, ht, &index);
                if (n > 0){
                    // ask user if they would like to update, delete or show a password using record number
                    manipulatePassword(reader, ht, index, key);
                }
                break;
            case 3:
                // view passwords
                viewPasswords(reader, ht);
                break;
            case 4:
                std::exit(0);
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

void viewPasswords(CSVReader& reader, HashTable& ht) {
    // display all passwords
    // look at all buckets and display them
    Bucket* buckets = ht.getBuckets();
    for (size_t i = 0; i < ht.numBuckets; i++) {
        Node* node = buckets[i].list.getHead();
        while (node != nullptr) {
            std::cout << "Domain: " << node->data[0] << "\n";
            std::cout << "Username: " << node->data[1] << "\n";
            std::cout << "Password: *********" << "\n";
            node = node->next;
        }
    }
}

void manipulatePassword(CSVReader& reader, HashTable& ht, Bucket* index, std::string& key) {
    std::string input;
    
    while (true) {
        std::cout << "Choose an option:\n";
        std::cout << "UPDATE/SHOW/DELETE <RECORD_NUMBER>\n";
        std::cout << "BACK - to exit this menu\n";
        std::cout << "Example: UPDATE 1\n";
        std::cout << "Enter your choice: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input); // read the full line of input

        std::istringstream iss(input);
        std::string command;
        int recordNumber = -1; // initialize with an invalid value

        iss >> command;
        // convert command to uppercase to handle case-insensitive comparison
        for (char& c : command) {
            c = toupper(static_cast<unsigned char>(c)); // ensure safe conversion
        }

        if (command == "BACK") {
            return; // exit the loop and function
        }

        iss >> recordNumber; // attempt to parse the record number

        if (command == "UPDATE" && recordNumber != -1) {
            updatePassword(reader, recordNumber, index, key);
            break;
        } else if (command == "SHOW" && recordNumber != -1) {
            showPassword(recordNumber, index, key);
            break; 
        } else if (command == "DELETE" && recordNumber != -1) {
            deletePassword(reader, recordNumber, index);
            break; 
        } else {
            // Handle invalid command or input format
            std::cout << "Invalid command or input format. Please try again.\n";
        }
    }
}

bool updatePassword(CSVReader& reader,int recordNumber, Bucket* index, std::string& key) {
    std::string username;
    std::string password;
    std::string prompt = "Enter new username: ";
    std::cout << prompt;
    std::cin >> username;
    prompt = "Enter new password: ";
    std::cout << prompt;
    std::cin >> password;
    // encrypt the password
    AESECB aes(key);
    password = aes.Encrypt(password);
    // update the password in the hash table
    std::cout << password << std::endl;
    std::cout << username << std::endl;
    Node* node = index->list.getHead();
    int counter = 0;
    while (node != nullptr) {
        counter++;
        if (counter == recordNumber) {
            std::cout << node->data[0] << std::endl;
            std::cout << node->data[1] << std::endl;
            if (reader.updateRow(node->data[0], node->data[1], password, username)){
                std::cout << "Record updated successfully.\n";
            }
            return true;
        }
        node = node->next;
    }
    std::cout << "Record not found.\n";
    return false;
}

void showPassword(int recordNumber, Bucket* index, std::string& key) {
    Node* node = index->list.getHead();
    int counter = 0;
    while (node != nullptr) {
        counter++;
        if (counter == recordNumber) {
            std::cout << "Record: " << counter << "\n";
            std::cout << "Username: " << node->data[1] << "\n";
            // decrypt the password
            AESECB aes(key);
            std::string password = aes.Decrypt(node->data[2]);
            std::cout << "Password: " << password << "\n";
            return;
        }
        node = node->next;
    }
    std::cout << "Record not found.\n";
}

bool deletePassword(CSVReader& reader, int recordNumber, Bucket* index) {
    Node* node = index->list.getHead();
    int counter = 0;
    while (node != nullptr) {
        counter++;
        if (counter == recordNumber) {
            reader.deleteRow(node->data[0], node->data[1]);
            std::cout << "Record deleted successfully.\n";
            return true;
        }
        node = node->next;
    }
    std::cout << "Record not found.\n";
    return false;
}

int searchPasswords(CSVReader& reader, HashTable& ht, Bucket** index) {
    std::string domain;
    std::string prompt = "Enter the domain: ";
    std::cout << prompt;
    std::cin >> domain;
    if (!ht.find(domain)) {
        std::cout << "No passwords found for " << domain << ".\n";
        return 0;
    }
    // look at all nodes in bucket and display them
    int counter = 0;
    Bucket* bucket = ht.getBucket(domain);
    Node* node = bucket->list.getHead();
    *index = bucket;
    while (node != nullptr) {
        counter++;
        if (node->data[0] == domain) {
            std::cout << "Record: " << counter << "\n";
            std::cout << "Username: " << node->data[1] << "\n";
            std::cout << "Password: *********" << "\n";
        }
        node = node->next;
    }
    std::cout << "Found " << counter << " passwords for " << domain << ".\n";
    return counter;
}

bool addPassword(CSVReader& reader, HashTable& ht, std::string& key) {
    // read data from user
    std::string domain;
    std::string username;
    std::string password;
    std::string prompt = "Enter the domain: ";
    std::cout << prompt;
    std::cin >> domain;
    prompt = "Enter the username: ";
    std::cout << prompt;
    std::cin >> username;
    prompt = "Enter the password: ";
    std::cout << prompt;
    std::cin >> password;
    // encrypt the password
    AESECB aes(key);
    password = aes.Encrypt(password);
    // add the password to the hash table
    reader.addRow(domain, username, password);

    std::cout << "Password added successfully.\n";
    return true;
}

void displayMenu2() {
    std::cout << "1. Add Password\n";
    std::cout << "2. Search Passwords\n";
    std::cout << "3. View Passwords\n";
    std::cout << "4. Exit\n";
}

void cleanup() {
    std::cout << "Cleaning up...\n";
    // encrypt unencrypted files
    if (KEY == "" || IV == "" || USER == "") {
        return;
    }
    // cast key and iv to SecByteBlock
    SecByteBlock keyblock((byte*)KEY.data(), KEY.size());
    SecByteBlock ivblock((byte*)IV.data(), IV.size());
    AESCrypto aesCrypto(keyblock, ivblock);
    // encrypt passwords file
    aesCrypto.EncryptFile("assets/" + USER + "_decrypted.csv", "assets/" + USER + ".csv");
    // remove unencrypted file
    std::remove(("assets/" + USER + "_decrypted.csv").c_str());
}

// signal handler for SIGINT
void signalHandler(int signal) {
    std::cout << " Signal " << signal << " caught.\n";
    // exiting will invoke cleanup()
    std::exit(EXIT_SUCCESS);
}

void clearCin() {
    // clear the error flag on cin (like if user entered a non-numeric value)
    std::cin.clear();
    // ignore everything in the buffer up to and including the next newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInt(std::string& prompt) {
    int choice;
    while (true) {
        std::cout << prompt;
        if (std::cin >> choice) {
            // if we successfully read a number, break out of the loop
            break;
        } else {
            std::cout << "Please enter a valid number.\n";
            clearCin(); // clear the error state and the input buffer
        }
    }
    return choice;
}

void displayInitMenu() {
    std::cout << "1. Create User\n";
    std::cout << "2. Log In\n";
    std::cout << "3. Exit\n";
}

bool createUser(std::string& key, std::string& iv, std::string& user) {
    std::string username;
    std::string password;
    while (true) {
        std::string prompt = "Enter your username: ";
        std::cout << prompt;
        std::cin >> username;
        if (username.length() < 4) {
            std::cout << "Username must be at least 4 characters long.\n";
            continue;
        }
        break;
    }
    while (true) {
        std::string prompt = "Enter your password: ";
        std::cout << prompt;
        std::cin >> password;
        if (password.length() < 8) {
            std::cout << "Password must be at least 8 characters long.\n";
            continue;
        }
        break;
    }
    UserHandler userHandler("assets/users.txt");
    if (userHandler.AddUser(username, password)) {
        std::cout << "User created successfully.\n";
        // generate key and iv values
        KeyIVGenerator keyIVGenerator;
        keyIVGenerator.GenerateKeyIV(password, key, iv);
        user = username;
        // make a new password file for the user
        std::string filename = "assets/" + username + ".csv";
        std::ofstream file(filename);
        SecByteBlock keyblock((byte*)key.data(), key.size());
        SecByteBlock ivblock((byte*)iv.data(), iv.size());
        AESCrypto aesCrypto(keyblock, ivblock);
        aesCrypto.EncryptFile(filename, filename);
        return true;
    } else {
        std::cout << "Failed to create user.\n";
        return false;
    }
}

bool verifyUser(std::string& key, std::string& iv, std::string& user) {
    std::string username;
    std::string password;
    std::string prompt = "Enter your username: ";
    std::cout << prompt;
    std::cin >> username;
    prompt = "Enter your password: ";
    std::cout << prompt;
    std::cin >> password;
    UserHandler userHandler("assets/users.txt");
    if (userHandler.VerifyUser(username, password)) {
        std::cout << "User verified successfully.\n";
        // generate key and iv values
        KeyIVGenerator keyIVGenerator;
        keyIVGenerator.GenerateKeyIV(password, key, iv);
        user = username;
        return true;
    } else {
        std::cout << "Failed to verify user.\n";
        return false;
    }
}
