#include "helpers.h"

// displays all password records stored for the user (passwords aren't displayed)
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

// handles commands to update, delete or show a certain password record
void manipulatePassword(CSVReader& reader, HashTable& ht, Bucket* index, std::string& key) {
    std::string input;
    // loop to handle invalid inputs
    while (true) {
        std::cout << "\n=== UPDATE/SHOW/DELETE a Password Record ===\n";
        std::cout << "Commands:\n";
        std::cout << "  UPDATE <RECORD_NUMBER>\n";
        std::cout << "  SHOW <RECORD_NUMBER>\n";
        std::cout << "  DELETE <RECORD_NUMBER>\n";
        std::cout << "  BACK - to exit this menu\n";
        std::cout << "Example: UPDATE 1\n";
        std::cout << "==========================================\n";
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
            // handle invalid command or input format
            std::cout << "Invalid command or input format. Please try again.\n";
        }
    }
}

// updates a specific password record
bool updatePassword(CSVReader& reader,int recordNumber, Bucket* index, std::string& key) {
    // read user input
    std::string username;
    std::string password;
    std::string prompt = "Enter new username: ";
    std::cout << prompt;
    std::cin >> username;
    prompt = "Enter new password: ";
    std::cout << prompt;
    std::cin >> password;

    // encrypt the new password
    AESECB aes(key);
    password = aes.Encrypt(password);

    // update the password in the hash table
    // get the record number-th node in the bucket
    // start by getting head of the linked list
    Node* node = index->list.getHead();
    // initialize a counter to keep track of the record number
    int counter = 0;
    while (node != nullptr) {
        counter++;
        // if we found the record number-th node, update the password
        if (counter == recordNumber) {
            if (reader.updateRow(node->data[0], node->data[1], password, username)){
                std::cout << "Record updated successfully.\n";
            }
            return true;
        }
        // else keep looking
        node = node->next;
    }
    // if we reach here, the record was not found
    std::cout << "Record not found.\n";
    return false;
}

// shows a specific password record
void showPassword(int recordNumber, Bucket* index, std::string& key) {
    // get the record number-th node in the bucket
    // start by getting head of the linked list
    Node* node = index->list.getHead();
    // initialize a counter to keep track of the record number
    int counter = 0;
    while (node != nullptr) {
        // increment on each iteration
        counter++;
        // if we found the record number-th node, display the username and password
        if (counter == recordNumber) {
            std::cout << "Record: " << counter << "\n";
            std::cout << "Username: " << node->data[1] << "\n";
            // decrypt the password
            AESECB aes(key);
            std::string password = aes.Decrypt(node->data[2]);
            std::cout << "Password: " << password << "\n";
            return;
        }
        // else keep looking
        node = node->next;
    }
    // if we reach here, the record was not found
    std::cout << "Record not found.\n";
}

// deletes a specific password record
bool deletePassword(CSVReader& reader, int recordNumber, Bucket* index) {
    // get the record number-th node in the bucket
    // start by getting head of the linked list
    Node* node = index->list.getHead();
    // initialize a counter to keep track of the record number
    int counter = 0;
    while (node != nullptr) {
        // increment on each iteration
        counter++;
        // if we found the record number-th node, delete the record
        if (counter == recordNumber) {
            reader.deleteRow(node->data[0], node->data[1]);
            std::cout << "Record deleted successfully.\n";
            return true;
        }
        // else keep looking
        node = node->next;
    }
    // if we reach here, the record was not found
    std::cout << "Record not found.\n";
    return false;
}

// searches for passwords for a specific domain (takes a pointer to a pointer in order to be able to change the value of the pointer in the calling function)
int searchPasswords(CSVReader& reader, HashTable& ht, Bucket** index) {
    // read data from user
    std::string domain;
    std::string prompt = "Enter the domain: ";
    std::cout << prompt;
    std::cin >> domain;
    // search for the domain in the hash table
    if (!ht.find(domain)) {
        std::cout << "No passwords found for " << domain << ".\n";
        return 0;
    }
    // look at all nodes in bucket and display them
    int counter = 0;
    // get the bucket for the domain
    Bucket* bucket = ht.getBucket(domain);
    // start by getting head of the linked list
    Node* node = bucket->list.getHead();
    // dereference the pointer to change its value to the bucket we want to return
    *index = bucket;
    while (node != nullptr) {
        // increment on each iteration
        counter++;
        // if we found the domain, display the username and masked password
        if (node->data[0] == domain) {
            std::cout << "Record: " << counter << "\n";
            std::cout << "Username: " << node->data[1] << "\n";
            std::cout << "Password: *********" << "\n";
            std::cout << "--------\n";
        }
        node = node->next;
    }
    // display the number of passwords found
    std::cout << "Found " << counter << " passwords for " << domain << ".\n";
    return counter;
}

// adds a password record
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

// displays the main menu after the user logs in
void displayMenu2() {
    std::cout << "\n--------------- Main Menu ---------------\n";
    std::cout << "1. Add Password\n";
    std::cout << "2. Search Passwords\n";
    std::cout << "3. View Passwords\n";
    std::cout << "4. Generate Password\n";
    std::cout << "5. Exit\n";
    std::cout << "-----------------------------------------\n";
}


// utility function that clears the input buffer
void clearCin() {
    // clear the error flag on cin (like if user entered a non-numeric value)
    std::cin.clear();
    // ignore everything in the buffer up to and including the next newline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// utility function to get an integer from the user
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

// displays the initial menu
void displayInitMenu() {
    std::cout << "\n================= Password Manager =================\n";
    std::cout << "1. Create User\n";
    std::cout << "2. Log In\n";
    std::cout << "3. Exit\n";
    std::cout << "====================================================\n";
}


// creates a new user
bool createUser(std::string& key, std::string& iv, std::string& user) {
    // read user input
    std::string username;
    std::string password;

    // verify username and password length
    std::cout << "\nCreating New User...\n";
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
    // add the user to the users file using the user handler
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
        // cast key and iv to SecByteBlock
        SecByteBlock keyblock((byte*)key.data(), key.size());
        SecByteBlock ivblock((byte*)iv.data(), iv.size());
        // encrypt the file
        AESCrypto aesCrypto(keyblock, ivblock);
        aesCrypto.EncryptFile(filename, filename);
        return true;
        std::cout << "User created successfully.\n";
        std::cout << "--------------------------------\n";
    }
    std::cout << "Failed to create user.\n";
    std::cout << "--------------------------------\n";
    return false;
}

// verifies an existing user
bool verifyUser(std::string& key, std::string& iv, std::string& user) {
    std::cout << "\nUser Verification Process...\n";
    // read user input
    std::string username;
    std::string password;
    std::string prompt = "Enter your username: ";
    std::cout << prompt;
    std::cin >> username;
    prompt = "Enter your password: ";
    std::cout << prompt;
    std::cin >> password;
    
    // verify the user using the user handler
    UserHandler userHandler("assets/users.txt");
    if (userHandler.VerifyUser(username, password)) {
        std::cout << "User verified successfully.\n";
        // generate key and iv values
        KeyIVGenerator keyIVGenerator;
        keyIVGenerator.GenerateKeyIV(password, key, iv);
        user = username;
        std::cout << "User verified successfully.\n";
        std::cout << "--------------------------------\n";
        return true;
    } else {
        std::cout << "Failed to verify user.\n";
        std::cout << "--------------------------------\n";
        return false;
    }
}

// generates a random password
void generatePassword(CSVReader& reader, HashTable& ht, std::string& key) {
    std::cout << "\nGenerating Secure Password...\n";
    // make a password generator object
    Generator generator;
    // read user input
    std::string domain;
    std::string username;
    std::string prompt = "Enter domain you want to generate password for: ";
    std::cout << prompt;
    std::cin >> domain;
    prompt = "Enter your username: ";
    std::cout << prompt;
    std::cin >> username;

    // generate a password of length 16
    std::string password = generator.generate(16);
    // display the generated password
    std::cout << "Generated password: " << password << "\n";
    // encrypt the password
    AESECB aes(key);
    password = aes.Encrypt(password);
    // add the password to the csv
    reader.addRow(domain, username, password);
    std::cout << "--------------------------------\n";
}