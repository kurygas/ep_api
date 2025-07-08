#include "crypto.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>

std::string Crypto::sha256(const std::string& input) {
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash;
    SHA256(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), hash.data());
    return joinHash(hash);
}

std::string Crypto::sha512(const std::string& input) {
    std::array<uint8_t, SHA512_DIGEST_LENGTH> hash;
    SHA512(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), hash.data());
    return joinHash(hash);
}

std::string Crypto::hmacSha256(const std::string& key, const std::string& data) {
    const auto hmac = HMAC(
        EVP_sha256(), 
        key.c_str(), 
        key.size(), 
        reinterpret_cast<const uint8_t*>(data.c_str()), 
        data.size(), 
        nullptr, 
        nullptr
    );
    std::string result;

    for (auto i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        result += std::format("{:02x}", result[i]);
    }

    return result;
}
