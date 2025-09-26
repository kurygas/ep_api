#pragma once

#include <unordered_map>

template<typename T>
class Storage : public std::unordered_map<int, T> {
public:
    static Storage<T>& instance() {
        static Storage<T> storage;
        return storage;
    }

    Storage(const Storage& other) = delete;
    Storage(Storage&& other) = delete;
    Storage& operator=(const Storage& other) = delete;
    Storage& operator=(Storage&& other) = delete;

private:
    Storage() = default;
    ~Storage() = default;
};
