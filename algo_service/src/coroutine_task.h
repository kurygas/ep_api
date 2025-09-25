#pragma once

#include <coroutine>

class Promise;

class Task : public std::coroutine_handle<Promise> {
public:
    using promise_type = Promise;

    explicit Task(const std::coroutine_handle<Promise>& handle);
};

enum class TaskType {
    Undefined = 0,
    Cf,
    Atc
};

class Promise {
public:
    Task get_return_object();
    std::suspend_never initial_suspend() const noexcept;
    std::suspend_always final_suspend() const noexcept;
    void return_value(int value);
    void unhandled_exception() const;

    int getPoint() const;
    int getUserId() const;
    int getSemesterId() const;
    TaskType getType() const;

    void setUserId(int userId);
    void setSemesterId(int semesterId);
    void setType(TaskType type);
    
private:
    int userId_ = -1;
    int semesterId_ = -1;
    int point_ = 0;
    TaskType type_ = TaskType::Undefined;
};
