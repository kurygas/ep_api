#include "utility_functions.h"

#include <random>
#include <format>
#include <openssl/sha.h>

std::string Utility::generateRandomString(const size_t size) {
    static const std::string pool = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::mt19937 generator((std::random_device())());
    std::string result(size, '\0');

    for (auto& ch : result) {
        ch = pool[(std::uniform_int_distribution<int>(0, pool.size() - 1))(generator)];
    }

    return result; 
}

std::string Utility::joinParams(const std::map<std::string, std::string>& params) {
    std::string result;

    for (const auto& [param, value] : params) {
        result += std::format("{}={}&", param, value);
    }

    result.pop_back();
    return result;
}

std::string Utility::sha512(const std::string& input) {
    std::array<uint8_t, SHA512_DIGEST_LENGTH> hash;
    SHA512(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), hash.data());
    std::string result;

    for (const auto& i : hash) {
        result += std::format("{:02x}", i);
    }

    return result;
}
