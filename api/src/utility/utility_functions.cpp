#include "utility_functions.h"
#include "str.h"

#include <format>

std::vector<std::string> Utility::split(const std::string& str, const char splitter) {
    std::string buf;
    std::vector<std::string> result;

    for (const auto& letter : str) {
        if (letter == splitter && !buf.empty()) {
            result.emplace_back(buf);
            buf.clear();
        }
        else if (letter != splitter) {
            buf.push_back(letter);
        }
    }

    if (!buf.empty()) {
        result.emplace_back(std::move(buf));
    }

    return result;
}
