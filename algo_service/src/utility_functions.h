#pragma once

#include <string>
#include <map>

namespace Utility {
    std::string generateRandomString(size_t size);
    std::string joinParams(const std::map<std::string, std::string>& params);
    std::string sha512(const std::string& input);
}
