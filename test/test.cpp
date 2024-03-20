#include <gtest/gtest.h>
#include "csvreader.h"

// make a new test
// name and purpose
TEST(CSVReaderTest, ReadsDataCorrectly) {
    std::string filename = "../assets/test_data.csv";
    CSVReader reader(filename);

    // read data
    auto data = reader.readData();

    // check that the data matches expected values
    EXPECT_EQ("John Doe", data[1][0]); 
    EXPECT_EQ("30", data[1][1]);
    EXPECT_EQ("Engineer", data[1][2]);

    EXPECT_EQ("Jane Doe", data[2][0]);
    EXPECT_EQ("29", data[2][1]);
    EXPECT_EQ("Doctor", data[2][2]); 
}

