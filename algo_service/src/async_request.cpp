#include "async_request.h"

GetAsync::GetAsync(cpr::Url url)
: url_(std::move(url)) {}

bool GetAsync::await_ready() const {
    return false;
}

void GetAsync::await_suspend(std::coroutine_handle<> handle) {
    cpr::GetCallback([this, handle = std::move(handle)](cpr::Response response) {
        response_ = std::move(response);
        handle.resume();
    }, std::move(url_), cpr::Timeout{5000});
}

cpr::Response GetAsync::await_resume() {
    return std::move(response_);
}
