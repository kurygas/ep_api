#include "utility_functions.h"
#include "str.h"

#include <format>

std::vector<std::string> Utility::split(const std::string& str) {
    std::string buf;
    std::vector<std::string> result;

    for (const auto& letter : str) {
        if (letter == '/' && !buf.empty()) {
            result.emplace_back(buf);
            buf.clear();
        }
        else if (letter != '/') {
            buf.push_back(letter);
        }
    }

    if (!buf.empty()) {
        result.emplace_back(std::move(buf));
    }

    return result;
}

std::string Utility::joinParams(const std::vector<std::pair<std::string, std::string>>& params) {
    std::string result;

    for (const auto& [param, value] : params) {
        result += std::format("{}={}&", param, value);
    }

    result.pop_back();
    return result;
}

std::string Utility::getFilepath(const int workResultId) {
    return std::format("{}/{}", Str::solutionsPath, std::to_string(workResultId));
}
