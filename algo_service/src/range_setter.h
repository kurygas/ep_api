#pragma once

#include "storage.h"

template<typename T>
class RangeSetter {
public:
    RangeSetter()
    : lock_(Storage<T>::instance().mutex_) {}

    template<typename U>
    void set(int id, U&& obj) {
        Storage<T>::instance().data_[id] = std::forward<U>(obj);
    }

private:
    const std::unique_lock<std::shared_mutex> lock_;
};
