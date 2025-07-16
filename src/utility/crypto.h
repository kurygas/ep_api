#pragma once

#include <string>
#include <array>
#include <format>

namespace Crypto {
    template<unsigned long HashSize>
    std::string joinHash(const std::array<uint8_t, HashSize>& hash) {
        std::string result;

        for (const auto& i : hash) {
            result += std::format("{:02x}", i);
        }

        return result;
    }

    std::string sha256(const std::string& input);
    std::string sha512(const std::string& input);
    std::string hmacSha256(const std::string& key, const std::string& data);
}
