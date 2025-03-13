#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

class JsonObject;
class User;
class Session;

class Resource : public Wt::WResource {
public:
    static JsonObject getBodyContent(const Wt::Http::Request& request);

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;

protected:
    virtual void processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, Session& session) const;
    virtual void processGet(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const;
    virtual void processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const;
};

class TokenResource : public Resource {
protected:
    void processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, Session& session) const override;
    void checkToken(const JsonObject& requestContent, Session& session) const;
};

class AdminResource : public TokenResource {
protected:
    void processRequest(const std::string& method, const JsonObject& requestContent, JsonObject& responseContent, Session& session) const override;
};