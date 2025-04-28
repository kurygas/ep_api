#include "request.h"

HttpRequest::HttpRequest(const Wt::Http::Request& request)
: tokenHeader_(request.headerValue("Authorization")) {
    setBody(request);
}

std::string HttpRequest::token() const {
    if (tokenHeader_.substr(0, 7) != "Bearer ") {
        throw std::runtime_error("Invalid token");
    }

    return tokenHeader_.substr(7);
}

const Wt::Json::Object& HttpRequest::body() const {
    return body_;
}

void HttpRequest::setBody(const Wt::Http::Request& request) {
    try {
        Wt::Json::parse(std::string(static_cast<std::istreambuf_iterator<char>>(request.in()), {}), body_);
    }
    catch (...) {}
}
