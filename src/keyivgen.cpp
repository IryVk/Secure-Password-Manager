#include "keyivgen.h"
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

// generate a key and iv using a password created by the user
void KeyIVGenerator::GenerateKeyIV(const std::string& password, std::string& outKey, std::string& outIV, size_t keyLength, size_t ivLength) {
    // the total size needed to store both key and IV
    size_t totalSize = keyLength + ivLength;

    // secure byte block for derived key and IV
    SecByteBlock derived(totalSize);

    // derive key and IV using PBKDF2 (SHA-256 as the underlying hash function here)
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    pbkdf.DeriveKey(
        derived.data(), derived.size(), 
        0x00, // purpose byte (unused)
        reinterpret_cast<const byte*>(password.data()), password.size(),
        nullptr, 0, // no salt
        10000, // iteration count
        0.0 // derived key time in seconds (unused)
    );

    // split the derived bytes into key and IV
    outKey.assign(reinterpret_cast<const char*>(derived.data()), keyLength);
    outIV.assign(reinterpret_cast<const char*>(derived.data() + keyLength), ivLength);
}
