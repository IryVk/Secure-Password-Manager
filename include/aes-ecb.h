#ifndef AESECB_H
#define AESECB_H

#include <string>

class AESECB {
public:
    // constructor that takes the encryption key
    AESECB(const std::string& key);

    // encrypts the given plaintext using AES-ECB and applies PKCS#7 padding
    std::string Encrypt(const std::string& plaintext);

    // decrypts the given ciphertext using AES-ECB and removes PKCS#7 padding
    std::string Decrypt(const std::string& ciphertext);

private:
    std::string key;
};

#endif // AESECB_H
