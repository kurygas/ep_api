#include "crypto.h"

#include <openssl/sha.h>

std::string Crypto::sha512(const std::string& input) {
    std::array<uint8_t, SHA512_DIGEST_LENGTH> hash;
    SHA512(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), hash.data());
    std::string result;

    for (const auto& i : hash) {
        result += std::format("{:02x}", i);
    }

    return result;
}
