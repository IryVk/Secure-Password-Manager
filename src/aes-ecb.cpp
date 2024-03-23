#include "aes-ecb.h"
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

AESECB::AESECB(const std::string& key) : key(key) {}

std::string AESECB::Encrypt(const std::string& plaintext) {
    std::string ciphertext;
    // setup AES key and ECB mode. No IV is needed for ECB.
    ECB_Mode<AES>::Encryption ecbEncryption((byte*)key.data(), key.size());
    
    // Use StringSource and StreamTransformationFilter for encryption with PKCS #7 padding
    StringSource ss(plaintext, true,
        new StreamTransformationFilter(ecbEncryption,
            new StringSink(ciphertext),
            BlockPaddingSchemeDef::PKCS_PADDING
        )
    );

    // make hex
    ciphertext = toHex(ciphertext);
    return ciphertext;
}

std::string AESECB::Decrypt(const std::string& ciphertext) {
    // convert from hex
    std::string ct = fromHex(ciphertext);
    std::string decryptedText;
    // setup AES key and ECB mode. No IV is needed for ECB.
    ECB_Mode<AES>::Decryption ecbDecryption((byte*)key.data(), key.size());

    // use StringSource and StreamTransformationFilter for decryption with PKCS #7 padding
    StringSource ss(ct, true,
        new StreamTransformationFilter(ecbDecryption,
            new StringSink(decryptedText),
            BlockPaddingSchemeDef::PKCS_PADDING
        )
    );

    return decryptedText;
}

std::string AESECB::toHex(const std::string& input) {
    std::string hex;
    StringSource ss(input, true,
        new HexEncoder(
            new StringSink(hex)
        )
    );
    return hex;
}

std::string AESECB::fromHex(const std::string& input) {
    std::string decoded;
    StringSource ss(input, true,
        new HexDecoder(
            new StringSink(decoded)
        )
    );
    return decoded;
}
