#pragma once

#include <vector>
#include <string>

namespace Utility {
    std::vector<std::string> split(const std::string& str, char splitter);
    std::string joinParams(const std::vector<std::pair<std::string, std::string>>& params);
}
