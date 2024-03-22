#include <gtest/gtest.h>
// #include "csvreader.h"
#include "linkedlist.h"
#include <sstream>
#include <string>
#include <hashtable.h>

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
    EXPECT_FALSE(list.remove("nonexistent"));
}

TEST_F(LinkedListTest, RemoveExistingElement) {
    list.add("key1", "", "");
    list.add("key2", "", "");
    EXPECT_TRUE(list.remove("key1"));
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
    ht->remove("testKey");
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


