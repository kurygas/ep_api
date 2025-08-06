#pragma once

#include <string>
#include <array>
#include <format>
#include <cstdint>
#include <openssl/sha.h>
#include <openssl/hmac.h>

namespace Crypto {
    std::string sha256(const std::string& input);
    std::string hmacSha256(const std::string& key, const std::string& data);
}
