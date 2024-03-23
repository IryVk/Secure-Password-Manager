#ifndef HELPERS_H
#define HELPERS_H

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

// helper functions

void clearCin(); // clear cin buffer
int getInt(std::string& prompt); // get integer input from user
void displayInitMenu(); // display initial menu
bool createUser(std::string& key, std::string& iv, std::string& user); // create a new user
bool verifyUser(std::string& key, std::string& iv, std::string& user); // verify an existing user
void displayMenu2(); // display menu for user
bool addPassword(CSVReader& reader, HashTable& ht, std::string& key); // add a new password
int searchPasswords(CSVReader& reader, HashTable& ht, Bucket** index); // search for a password
void manipulatePassword(CSVReader& reader, HashTable& ht, Bucket* index, std::string& key); // manipulate a password
bool updatePassword(CSVReader& reader, int recordNumber, Bucket* index, std::string& key); // update a password
void showPassword(int recordNumber, Bucket* index, std::string& key); // show a password
bool deletePassword(CSVReader& reader, int recordNumber, Bucket* index); // delete a password
void viewPasswords(CSVReader& reader, HashTable& ht); // view all passwords
void generatePassword(CSVReader& reader, HashTable& ht, std::string& key); // generate a password

#endif // HELPERS_H