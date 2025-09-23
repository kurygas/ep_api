#pragma once

#include <coroutine>

class Promise;

class Task : public std::coroutine_handle<Promise> {
public:
    using promise_type = Promise;

    explicit Task(const std::coroutine_handle<Promise>& handle);
};

class Promise {
public:
    Task get_return_object();
    std::suspend_never initial_suspend() const noexcept;
    std::suspend_always final_suspend() const noexcept;
    void return_value(int value);
    int get_value() const;
    void unhandled_exception() const;
    
private:
    int value_;
};
