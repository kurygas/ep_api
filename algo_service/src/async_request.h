#pragma once

#include <coroutine>
#include <cpr/cpr.h>

class GetAsync {
public:
    explicit GetAsync(cpr::Url url);
    bool await_ready() const;
    void await_suspend(std::coroutine_handle<> handle);
    cpr::Response await_resume();

private:
    const cpr::Url url_;
    cpr::Response response_;
};
