#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <string>
#include "cryptlib.h"
#include "secblock.h"

class AESCrypto {
public:
    AESCrypto(const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv); // constructor
    void EncryptFile(const std::string& inputFilename, const std::string& outputFilename); // encrypt a file
    void DecryptFile(const std::string& inputFilename, const std::string& outputFilename); // decrypt a file

private:
    CryptoPP::SecByteBlock key; // key
    CryptoPP::SecByteBlock iv; // initialization vector
};

#endif // AESCRYPTO_H
