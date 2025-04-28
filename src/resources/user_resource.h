#pragma once

#include "resource.h"

class UserResource : public Resource {
protected:
    void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session, Wt::Dbo::Transaction& tr) const override;
    void processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const override;
    void processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const override;
    void processPatch(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id) const override;
    void processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, int id, 
        const std::string& method) const override;

private:
    Wt::Json::Object getJson(const Wt::Dbo::ptr<User>& user) const;
};
