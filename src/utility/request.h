#pragma once

#include <Wt/Http/Request.h>

#include "json.h"

class HttpRequest {
public:
    explicit HttpRequest(const Wt::Http::Request& request);

    std::string token() const;
    const Wt::Json::Object& body() const;

private:
    void setBody(const Wt::Http::Request& request);

    const std::string tokenHeader_;
    Wt::Json::Object body_;
};
