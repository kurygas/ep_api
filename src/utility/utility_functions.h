#pragma once

#include <vector>
#include <string>

namespace Utility {
    std::vector<std::string> split(const std::string& str);
    int getId(const std::vector<std::string>& path);
    const std::string& getMethod(const std::vector<std::string>& path);

    template<typename T>
    bool isIntersect(const T& l1, const T& r1, const T& l2, const T& r2) {
        return l1 <= r2 && l2 <= r1;
    }
}
