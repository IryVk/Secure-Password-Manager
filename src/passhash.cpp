#include "passhash.h"
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;

// Generate a random salt of the given length
std::string PasswordHasher::GenerateRandomSalt(size_t length) {
    AutoSeededRandomPool rng;
    byte salt[length];
    rng.GenerateBlock(salt, sizeof(salt));
    return ToHexString(salt, sizeof(salt));
}

// Hash the password using SHA-256 and the given salt
std::string PasswordHasher::HashPassword(const std::string& password, const std::string& salt) {
    SHA256 hash;
    std::string digest;
    StringSource s(password + salt, true, new HashFilter(hash, new HexEncoder(new StringSink(digest), false)));
    return salt + digest; // Prepend the salt to the hash for storage
}

// Verify if the provided password matches the expected hash when using the same salt
bool PasswordHasher::VerifyPassword(const std::string& password, const std::string& saltedHash) {
    size_t saltLength = 32; // Length of the salt in hex characters (16 bytes * 2 characters per byte)
    std::string salt = saltedHash.substr(0, saltLength);
    std::string expectedHash = saltedHash.substr(saltLength);

    std::string actualHash = HashPassword(password, salt).substr(saltLength); // Hash and strip the salt
    return actualHash == expectedHash;
}

// Convert binary data to a hex string
std::string PasswordHasher::ToHexString(const unsigned char* data, size_t length) {
    std::string hex;
    HexEncoder encoder(new StringSink(hex));
    encoder.Put(data, length);
    encoder.MessageEnd();
    return hex;
}
