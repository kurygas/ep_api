#pragma once

#include <optional>
#include <Wt/Http/Request.h>

#include "json.h"

class HttpRequest {
public:
    explicit HttpRequest(const Wt::Http::Request& request);

    std::string getToken() const;
    const JsonObject& getBody() const;
    const std::string& getParam(const std::string& key) const;
    const std::string& getMethod() const;

private:
    void setBody(const Wt::Http::Request& request);

    const std::string method_;
    const Wt::Http::ParameterMap& parameterMap_;
    const std::string tokenHeader_;
    JsonObject body_;
};