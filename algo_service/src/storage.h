#pragma once

#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

template<typename T>
class RangeSetter;

template<typename T>
class Storage {
public:
    friend class RangeSetter<T>; 

    static Storage& instance() {
        static Storage storage;
        return storage;
    }

    Storage(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    User get(int id) const {
        const std::shared_lock<std::shared_mutex> lock(mutex_);
        return data_.at(id);
    }

    template<typename U>
    void set(int id, U&& obj) {
        const std::unique_lock<std::shared_mutex> lock(mutex_);
        data_[id] = std::forward<U>(obj);
    }

    void del(int id) {
        const std::unique_lock<std::shared_mutex> lock(mutex_);
        data_.erase(id);
    }

private:
    Storage() = default;
    ~Storage() = default;

    std::unordered_map<int, T> data_;
    mutable std::shared_mutex mutex_;
};
