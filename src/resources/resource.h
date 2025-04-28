#pragma once

#include <Wt/WResource.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/collection.h>
#include <Wt/Dbo/Transaction.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

#include "json.h"

class Session;
class User;
class HttpRequest;

class Resource : public Wt::WResource {
public:
    void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override;

protected:
    virtual void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const;
    virtual void processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const;
    
    virtual void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session, Wt::Dbo::Transaction& tr) const;
    virtual void processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id, 
        const std::string& method) const;

    virtual void processPatch(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const;
    virtual void processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const;

    static Wt::Dbo::ptr<User> checkAuth(const HttpRequest& request, Session& session);
    static Wt::Dbo::ptr<User> checkTeacher(const HttpRequest& request, Session& session);
    static Wt::Dbo::ptr<User> checkAdmin(const HttpRequest& request, Session& session);
    static Wt::Dbo::ptr<User> checkAuthId(const HttpRequest& request, Session& session, int id);

private:
    static std::vector<std::string> split(const std::string& str);
    static int getId(const std::vector<std::string>& path);
    static const std::string& getMethod(const std::vector<std::string>& path);
};
