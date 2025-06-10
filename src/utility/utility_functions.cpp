#include "utility_functions.h"

std::vector<std::string> Utility::split(const std::string& str) {
    std::string buf;
    std::vector<std::string> result;

    for (const auto& letter : str) {
        if (letter == '/') {
            result.emplace_back(buf);
            buf.clear();
        }
        else {
            buf.push_back(letter);
        }
    }

    if (!buf.empty()) {
        result.emplace_back(buf);
    }

    return result;
}

int Utility::getId(const std::vector<std::string>& path) {
    return std::stoi(path[0]);
}

const std::string& Utility::getMethod(const std::vector<std::string>& path) {
    return path[1];
}