#ifndef KEYIVGENERATOR_H
#define KEYIVGENERATOR_H

#include <string>

class KeyIVGenerator {
public:
    // generates a key and IV for AES encryption from a given password and salt.
    // the key and IV lengths are specified in bytes. For AES-256, keyLength should be 32 bytes, and ivLength typically is 16 bytes.
    static void GenerateKeyIV(const std::string& password, std::string& outKey, std::string& outIV, size_t keyLength=32, size_t ivLength=16);
};

#endif // KEYIVGENERATOR_H
