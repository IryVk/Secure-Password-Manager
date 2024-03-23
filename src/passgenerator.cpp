#include "passgenerator.h"
#include <random>

// allowed characters for the password
static const char kAllowedChars[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!@#$%^&*()_+-=[]{}|;':,.<>/?";

// generate a random password of the specified length
std::string Generator::generate(size_t length) {
    std::string password;

    // use a random device to seed a Mersenne Twister engine.
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, sizeof(kAllowedChars) - 2); // -2 to account for the null terminator

    // generate a random password of the specified length
    for (size_t i = 0; i < length; ++i) {
        password += kAllowedChars[distribution(generator)];
    }

    return password;
}
