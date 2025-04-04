#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

class JsonObject;
class Session;
class User;

class Resource : public Wt::WResource {
public:
    static JsonObject getBodyContent(const Wt::Http::Request& request);
    static std::string getToken(const Wt::Http::Request& request);
    static const std::string& getParameter(const Wt::Http::Request& request, const std::string& key);

    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;

protected:
    virtual void processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const;
    virtual void processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const;
    virtual void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const;
};

class TokenResource : public Resource {
protected:
    void processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
};

class AdminResource : public TokenResource {
protected:
    void processRequest(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
};