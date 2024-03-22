#include "aes-gcm.h"
#include <fstream>
#include <stdexcept>
#include "files.h"
#include "aes.h"
#include "gcm.h"
#include "filters.h"

using namespace CryptoPP;

AESCrypto::AESCrypto(const SecByteBlock& key, const SecByteBlock& iv) : key(key), iv(iv) {}

void AESCrypto::EncryptFile(const std::string& inputFilename, const std::string& outputFilename) {
    GCM<AES>::Encryption encryption;
    encryption.SetKeyWithIV(key, key.size(), iv);

    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Error opening source file for encryption: " + inputFilename);
    }

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Error opening target file for encryption: " + outputFilename);
    }

    FileSource fs(inputFilename.c_str(), true,
        new AuthenticatedEncryptionFilter(encryption,
            new FileSink(outputFile)
        ) 
    ); 
}

void AESCrypto::DecryptFile(const std::string& inputFilename, const std::string& outputFilename) {
    GCM<AES>::Decryption decryption;
    decryption.SetKeyWithIV(key, key.size(), iv);

    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Error opening source file for decryption: " + inputFilename);
    }

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("Error opening target file for decryption: " + outputFilename);
    }

    FileSource fs(inputFilename.c_str(), true,
        new AuthenticatedDecryptionFilter(decryption,
            new FileSink(outputFile),
            AuthenticatedDecryptionFilter::THROW_EXCEPTION
        ) 
    ); 
}
