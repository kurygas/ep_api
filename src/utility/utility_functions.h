#pragma once

#include <vector>
#include <string>

namespace Utility {
    std::vector<std::string> split(const std::string& str);
    std::string joinParams(const std::vector<std::pair<std::string, std::string>>& params);
    std::string getFilepath(int workResultId);
}
