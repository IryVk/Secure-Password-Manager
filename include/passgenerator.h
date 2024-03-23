#ifndef SECURE_PASSWORD_GENERATOR_H
#define SECURE_PASSWORD_GENERATOR_H

#include <string>

class Generator {
public:
    // generates a secure random password of the given length
    std::string generate(size_t length=16);
};

#endif // SECURE_PASSWORD_GENERATOR_H
