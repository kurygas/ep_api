#include "request.h"
#include "http_exceptions.h"

HttpRequest::HttpRequest(const Wt::Http::Request& request)
: request_(request) {
    setBody();
}

std::string HttpRequest::token() const
{
    const auto tokenHeader = request_.headerValue("Authorization");

    if (tokenHeader.substr(0, 7) != "Bearer ") {
        throw AuthException("Invalid token");
    }

    return tokenHeader.substr(7);
}

const Wt::Json::Object& HttpRequest::body() const {
    return body_;
}

const Wt::Http::UploadedFile* HttpRequest::file(const std::string& name) const {
    return request_.getUploadedFile(name);
}

void HttpRequest::setBody() {
    try {
        Wt::Json::parse(std::string(static_cast<std::istreambuf_iterator<char>>(request_.in()), {}), body_);
    }
    catch (...) {}
}
