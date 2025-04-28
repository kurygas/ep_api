#pragma once

#include "resource.h"

class UserResource : public Resource {
protected:
    void processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    void processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const override;
    void processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const override;

private:
    Wt::Json::Object getJson(const Wt::Dbo::ptr<User>& user) const;
};
