#include "passgenerator.h"
#include <iostream>

int main() {
    Generator generator;
    std::string password = generator.generate(12); // Generate a 12-character password
    std::cout << "Generated password: " << password << std::endl;
    return 0;
}