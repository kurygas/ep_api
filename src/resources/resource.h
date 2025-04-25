#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

class JsonObject;
class Session;
class User;
class HttpRequest;

class Resource : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;

protected:
    virtual void processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const;
    virtual void processGet(const HttpRequest& request, JsonObject& responseContent, Session& session) const;
    virtual void processPost(const HttpRequest& request, JsonObject& responseContent, Session& session) const;
};