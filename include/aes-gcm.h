#ifndef AESCRYPTO_H
#define AESCRYPTO_H

#include <string>
#include "cryptlib.h"
#include "secblock.h"

class AESCrypto {
public:
    AESCrypto(const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv);
    void EncryptFile(const std::string& inputFilename, const std::string& outputFilename);
    void DecryptFile(const std::string& inputFilename, const std::string& outputFilename);

private:
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;
};

#endif // AESCRYPTO_H
