#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <Wt/Json/Array.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Value.h>
#include <Wt/Json/Serializer.h>
#include <Wt/Json/Parser.h>

class Session;
class User;
class HttpRequest;

class Resource : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;

protected:
    virtual void processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const;
    virtual void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const;
    virtual void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const;
};

class TokenResource : public Resource {
protected:
    void processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
};

class AdminResource : public TokenResource {
protected:
    void processRequest(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
};
