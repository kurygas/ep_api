#pragma once

#include <Wt/Json/Array.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Value.h>
#include <Wt/Json/Serializer.h>
#include <Wt/Json/Parser.h>
#include <Wt/Http/Request.h>

class HttpRequest {
public:
    explicit HttpRequest(const Wt::Http::Request& request);

    std::string getToken() const;
    const Wt::Json::Object& getBody() const;
    const Wt::WString& getParam(const std::string& key) const;
    const Wt::WString& getMethod() const;

private:
    void setBody(const Wt::Http::Request& request);

    const Wt::WString method_;
    const Wt::Http::ParameterMap& parameterMap_;
    const std::string tokenHeader_;
    Wt::Json::Object body_;
};