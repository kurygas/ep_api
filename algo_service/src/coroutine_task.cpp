#include "coroutine_task.h"

#include <exception>

Task::Task(const std::coroutine_handle<Promise>& handle)
: std::coroutine_handle<Promise>(handle) {}

Task Promise::get_return_object() {
    return Task(std::coroutine_handle<Promise>::from_promise(*this));
}

std::suspend_never Promise::initial_suspend() const {
    return {};
}

std::suspend_always Promise::final_suspend() const {
    return {};
}

void Promise::return_value(const int value) {
    value_ = value;
}

int Promise::get_value() const {
    return value_;
}

void Promise::unhandled_exception() const {}
