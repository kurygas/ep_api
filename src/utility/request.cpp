#include "request.h"

HttpRequest::HttpRequest(const Wt::Http::Request& request)
: method_(request.method())
, parameterMap_(request.getParameterMap())
, tokenHeader_(request.headerValue("Authorization")) {
    setBody(request);
}

std::string HttpRequest::getToken() const {
    if (tokenHeader_.substr(0, 7) != "Bearer ") {
        throw std::runtime_error("Invalid token");
    }

    return tokenHeader_.substr(7);
}

const Wt::Json::Object& HttpRequest::getBody() const {
    return body_;
}

const Wt::WString& HttpRequest::getParam(const std::string& key) const {
    const auto& values = parameterMap_.at(key);

    if (values.empty()) {
        throw std::runtime_error("HttpRequest does not contain key " + key);
    }

    return values[0];
}

const Wt::WString& HttpRequest::getMethod() const {
    return method_;
}

void HttpRequest::setBody(const Wt::Http::Request& request) {
    try {
        Wt::Json::parse(std::string(static_cast<std::istreambuf_iterator<char>>(request.in()), {}), body_);
    }
    catch (...) {
        throw std::runtime_error("Invalid payload");
    }
}
