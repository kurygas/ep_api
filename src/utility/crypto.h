#pragma once

#include <string>

namespace Crypto {
    std::string sha256(const std::string& input);
    std::string hmacSha256(const std::string& key, const std::string& data);
}
