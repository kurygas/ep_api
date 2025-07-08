#pragma once

#include <vector>
#include <string>

namespace Utility {
    std::vector<std::string> split(const std::string& str);
    std::string joinParams(const std::vector<std::pair<std::string, std::string>>& params);
    int getId(const std::vector<std::string>& path);
    const std::string& getMethod(const std::vector<std::string>& path);
}
