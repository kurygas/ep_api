#pragma once

#include <vector>
#include <string>

namespace Utility {
    std::vector<std::string> split(const std::string& str);
    int getId(const std::vector<std::string>& path);
    const std::string& getMethod(const std::vector<std::string>& path);
}
