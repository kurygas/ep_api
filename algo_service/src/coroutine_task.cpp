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
    point_ = value;
}

void Promise::unhandled_exception() const {}

int Promise::getPoint() const {
    return point_;
}

int Promise::getUserId() const {
    return userId_;
}

int Promise::getSemesterId() const {
    return semesterId_;
}

TaskType Promise::getType() const {
    return type_;
}

void Promise::setUserId(const int userId) {
    userId_ = userId;
}

void Promise::setSemesterId(const int semesterId) {
    semesterId_ = semesterId;
}

void Promise::setType(const TaskType type) {
    type_ = type;
}
