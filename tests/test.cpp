#include <gtest/gtest.h>
#include "csvreader.h"
#include "linkedlist.h"
#include "aes-gcm.h"
#include <sstream>
#include <string>
#include <hashtable.h>
#include <fstream>
#include <filesystem>
#include "aes-gcm.h"
#include "cryptlib.h"
#include "osrng.h"
#include "passhash.h"
#include "userhandler.h"

using namespace CryptoPP;

// utility function to capture std::cout output
std::string capturePrintOutput(LinkedList& list) {
    std::stringstream buffer;
    std::streambuf* prevCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    list.print();

    std::cout.rdbuf(prevCoutBuf); // reset to standard output
    return buffer.str();
}

// ==================== LinkedList Tests ====================
// initialize a test fixture
class LinkedListTest : public ::testing::Test {
protected:
    LinkedList list;
};

TEST_F(LinkedListTest, IsEmptyInitially) {
    EXPECT_TRUE(list.isEmpty());
}

TEST_F(LinkedListTest, IsNotEmptyAfterAdd) {
    list.add("data1", "data2", "data3");
    EXPECT_FALSE(list.isEmpty());
}

TEST_F(LinkedListTest, SizeIsCorrectAfterMultipleAdds) {
    list.add("data1", "data2", "data3");
    list.add("data4", "data5", "data6");
    EXPECT_EQ(list.size(), 2);
}

TEST_F(LinkedListTest, PrintSingleElement) {
    list.add("A", "B", "C");
    std::string expectedOutput = "A, B, C -> nullptr\n";
    EXPECT_EQ(capturePrintOutput(list), expectedOutput);
}

TEST_F(LinkedListTest, PrintMultipleElements) {
    list.add("A", "B", "C");
    list.add("D", "E", "F");
    std::string expectedOutput = "A, B, C -> D, E, F -> nullptr\n";
    EXPECT_EQ(capturePrintOutput(list), expectedOutput);
}

TEST_F(LinkedListTest, FindNonExistentElement) {
    EXPECT_FALSE(list.find("nonexistent"));
}

TEST_F(LinkedListTest, FindExistingElement) {
    list.add("key1", "", "");
    list.add("key2", "", "");
    EXPECT_TRUE(list.find("key1"));
    EXPECT_TRUE(list.find("key2"));
}

TEST_F(LinkedListTest, RemoveNonExistentElement) {
    EXPECT_FALSE(list.remove("nonexistent", ""));
}

TEST_F(LinkedListTest, RemoveExistingElement) {
    list.add("key1", "", "");
    list.add("key2", "", "");
    EXPECT_TRUE(list.remove("key1", ""));
    EXPECT_FALSE(list.find("key1")); // verify key1 is no longer in the list
    EXPECT_TRUE(list.find("key2")); // verify key2 is still in the list
}

TEST_F(LinkedListTest, GetHead) {
    list.add("key1", "", "");
    list.add("key2", "", "");
    Node* head = list.getHead();
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data[0], "key1");
}

TEST_F(LinkedListTest, GetNode) {
    list.add("domain.com", "user", "pass");
    Node* node = list.getNode("domain.com", "user");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data[0], "domain.com");
    EXPECT_EQ(node->data[1], "user");
    EXPECT_EQ(node->data[2], "pass");
}

TEST_F(LinkedListTest, UpdateNonExistentElement) {
    EXPECT_FALSE(list.update("nonexistent", "newDomain", "newUser", "newPass"));
}

TEST_F(LinkedListTest, UpdateExistingElement) {
    list.add("domain.com", "user", "pass");
    EXPECT_TRUE(list.update("domain.com", "user", "newPass"));
    // access the node and verify the data has been updated
    Node* node = list.getNode("domain.com", "user");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data[2], "newPass");
}

// ==================== HashTable Tests ====================
class HashTableTest : public ::testing::Test {
protected:
    HashTable* ht;
    size_t numBuckets = 10; // choose an appropriate size for testing

    void SetUp() override {
        ht = new HashTable(numBuckets);
    }
};

// test for constructor and destructor
TEST_F(HashTableTest, HashTableCreation) {
    ASSERT_NE(ht, nullptr);
}

// test inserting and finding a key
TEST_F(HashTableTest, InsertAndFind) {
    ht->insert("testKey");
    EXPECT_TRUE(ht->find("testKey"));
}

// test finding a non-existent key
TEST_F(HashTableTest, FindNonExistent) {
    EXPECT_FALSE(ht->find("nonExistentKey"));
}

// test removing a key
TEST_F(HashTableTest, InsertAndRemove) {
    ht->insert("testKey");
    EXPECT_TRUE(ht->find("testKey"));
    ht->remove("testKey", "");
    EXPECT_FALSE(ht->find("testKey"));
}

// test inserting at a specific index
TEST_F(HashTableTest, InsertAtIndexAndFind) {
    ht->insertAtIndex(3, "domain.com", "user", "pass");
    // verify the data was inserted at the correct index
    Bucket* buckets = ht->getBuckets();
    LinkedList list = buckets[3].list;
    EXPECT_TRUE(list.find("domain.com"));
}

// test updating a record
TEST_F(HashTableTest, Update) {
    ht->insert("newDomain.com", "user", "pass");
    ht->update("newDomain.com", "user", "newPass", "newUser");
    // verify the data was updated
    Bucket* bucket = ht->getBucket("newDomain.com");
    ASSERT_NE(bucket, nullptr);
    Node* node = bucket->list.getNode("newDomain.com", "newUser");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data[1], "newUser");
    EXPECT_EQ(node->data[2], "newPass");
}

// ==================== CSVReader Tests ====================
namespace fs = std::filesystem;

class CSVReaderTest : public ::testing::Test {
protected:
    HashTable hashtable;
    std::string testFilename = "../assets/test_data.csv"; // Path to your test CSV file

    void SetUp() override {
        // create a temporary CSV file for testing
        std::ofstream outFile(testFilename);
        outFile << "495676536135009626,domain1.com,user1,password1\n";
        outFile << "495676536136883787,domain2.com,user2,password2\n";
        outFile << "495676536138757948,domain3.com,user3,password3\n";
        outFile.close();
    }

    void TearDown() override {
        // Cleanup: remove the test file
        fs::remove(testFilename);
    }
};

TEST_F(CSVReaderTest, LoadCSV) {
    CSVReader reader(hashtable, testFilename);
    reader.load();
    EXPECT_TRUE(hashtable.find("domain1.com"));
}

TEST_F(CSVReaderTest, SaveCSV) {
    CSVReader reader(hashtable, testFilename);
    reader.load();

    // modify the hashtable
    reader.addRow("domain4.com", "user4", "password4");
    reader.save();

    // reload and check if the new row exists
    HashTable newHashTable;
    CSVReader newReader(newHashTable, testFilename);
    newReader.load();
    EXPECT_TRUE(newHashTable.find("domain4.com"));
}

TEST_F(CSVReaderTest, AddRow) {
    CSVReader reader(hashtable, testFilename);
    reader.addRow("domain4.com", "user4", "password4");
    reader.save();

    // verify addition
    HashTable newHashTable;
    CSVReader newReader(newHashTable, testFilename);
    newReader.load();
    EXPECT_TRUE(newHashTable.find("domain4.com"));
}

TEST_F(CSVReaderTest, UpdateRow) {
    CSVReader reader(hashtable, testFilename);
    reader.load();
    reader.updateRow("domain1.com", "user1", "newPassword");
    reader.save();

    // verify update
    HashTable newHashTable;
    CSVReader newReader(newHashTable, testFilename);
    newReader.load();
    Bucket* bucket = newHashTable.getBucket("domain1.com");
    ASSERT_NE(bucket, nullptr);
    Node* node = bucket->list.getNode("domain1.com", "user1");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data[2], "newPassword");
}

TEST_F(CSVReaderTest, DeleteRow) {
    CSVReader reader(hashtable, testFilename);
    reader.deleteRow("domain1.com", "user1");
    reader.save();

    // verify deletion
    HashTable newHashTable;
    CSVReader newReader(newHashTable, testFilename);
    newReader.load();
    EXPECT_FALSE(newHashTable.find("domain1.com"));
}

// ==================== AES-GCM Tests ====================
class AESCryptoTest : public ::testing::Test {
protected:
    std::string plaintextFilename = "plaintext.tmp";
    std::string encryptedFilename = "encrypted.tmp";
    std::string decryptedFilename = "decrypted.tmp";
    AESCrypto* crypto = nullptr;

    void SetUp() override {
        // generate a random key and IV for testing
        AutoSeededRandomPool prng;
        SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        prng.GenerateBlock(key, key.size());

        SecByteBlock iv(AES::BLOCKSIZE);
        prng.GenerateBlock(iv, iv.size());

        crypto = new AESCrypto(key, iv);

        // create and write to the plaintext file
        std::ofstream out(plaintextFilename);
        out << "This is a test string to encrypt and decrypt using AES-GCM.";
        out.close();
    }

    void TearDown() override {
        delete crypto;
        // Remove temporary files
        remove(plaintextFilename.c_str());
        remove(encryptedFilename.c_str());
        remove(decryptedFilename.c_str());
    }
};

TEST_F(AESCryptoTest, EncryptDecryptFile) {
    // encrypt the plaintext file
    crypto->EncryptFile(plaintextFilename, encryptedFilename);

    // decrypt the encrypted file
    crypto->DecryptFile(encryptedFilename, decryptedFilename);

    // eead decrypted content
    std::ifstream in(decryptedFilename);
    std::string decrypted((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // read original content for comparison
    std::ifstream inOrig(plaintextFilename);
    std::string original((std::istreambuf_iterator<char>(inOrig)), std::istreambuf_iterator<char>());
    inOrig.close();

    // compare decrypted content with the original plaintext
    EXPECT_EQ(original, decrypted);
}

// ==================== Hash Tests ====================
class PasswordHasherTest : public ::testing::Test {
protected:
};

// tests that the generated salt is of the correct length
TEST_F(PasswordHasherTest, GeneratesCorrectSaltLength) {
    auto salt = PasswordHasher::GenerateRandomSalt(16); // 16 bytes = 32 hex characters
    EXPECT_EQ(salt.length(), 32u);
}

// tests that generated salts are unique
TEST_F(PasswordHasherTest, GeneratesUniqueSalts) {
    std::set<std::string> salts;
    const size_t numSalts = 100;
    for (size_t i = 0; i < numSalts; ++i) {
        auto salt = PasswordHasher::GenerateRandomSalt(16);
        salts.insert(salt);
    }
    EXPECT_EQ(salts.size(), numSalts);
}

// tests that HashPassword includes the salt in its output and produces a consistent hash for the same input
TEST_F(PasswordHasherTest, HashIncludesSaltAndIsConsistent) {
    std::string password = "testPassword";
    std::string salt = PasswordHasher::GenerateRandomSalt(16); // generate a 16-byte salt
    auto hashWithSalt = PasswordHasher::HashPassword(password, salt);
    // check if the salt is at the start of the hash
    EXPECT_EQ(hashWithSalt.substr(0, 32), salt);
    // hash again to check for consistency
    auto sameHashWithSalt = PasswordHasher::HashPassword(password, salt);
    EXPECT_EQ(hashWithSalt, sameHashWithSalt);
}

// tests that VerifyPassword returns true for correct password
TEST_F(PasswordHasherTest, VerifiesCorrectPassword) {
    std::string password = "correctPassword";
    std::string salt = PasswordHasher::GenerateRandomSalt(16); // generate a 16-byte salt
    auto hashWithSalt = PasswordHasher::HashPassword(password, salt);
    EXPECT_TRUE(PasswordHasher::VerifyPassword(password, hashWithSalt));
}

// tests that VerifyPassword returns false for incorrect password
TEST_F(PasswordHasherTest, FailsIncorrectPassword) {
    std::string correctPassword = "correctPassword";
    std::string incorrectPassword = "wrongPassword";
    std::string salt = PasswordHasher::GenerateRandomSalt(16); // Generate a 16-byte salt
    auto hashWithSalt = PasswordHasher::HashPassword(correctPassword, salt);
    EXPECT_FALSE(PasswordHasher::VerifyPassword(incorrectPassword, hashWithSalt));
}

// ==================== UserHandler Tests ====================
// fixture class for UserHandler tests
class UserHandlerTest : public ::testing::Test {
protected:
    std::string testFilename = "test_users.txt";

    void SetUp() override {
        // ensure the file is empty before each test
        std::ofstream ofs(testFilename, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }

    void TearDown() override {
        // remove the test file after each test
        std::remove(testFilename.c_str());
    }
};

// test adding a new user
TEST_F(UserHandlerTest, AddUser) {
    UserHandler handler(testFilename);
    ASSERT_TRUE(handler.AddUser("testUser", "password"));

    // verify that the file contains the new user
    std::ifstream file(testFilename);
    std::string line;
    std::getline(file, line);
    file.close();

    // the line should contain the username and a hashed password
    ASSERT_TRUE(line.find("testUser") != std::string::npos);
}

// test verifying a user with the correct password
TEST_F(UserHandlerTest, VerifyCorrectPassword) {
    UserHandler handler(testFilename);
    handler.AddUser("testUser", "password");

    ASSERT_TRUE(handler.VerifyUser("testUser", "password"));
}

// test verifying a user with an incorrect password
TEST_F(UserHandlerTest, VerifyIncorrectPassword) {
    UserHandler handler(testFilename);
    handler.AddUser("testUser", "correctPassword");

    ASSERT_FALSE(handler.VerifyUser("testUser", "wrongPassword"));
}

// test handling of a non-existing user
TEST_F(UserHandlerTest, NonExistingUser) {
    UserHandler handler(testFilename);
    ASSERT_FALSE(handler.VerifyUser("nonExistingUser", "password"));
}