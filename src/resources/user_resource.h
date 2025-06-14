#pragma once

#include "resource.h"
#include "user.h"

class UserResource : public Resource<User> {
protected:
    void processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, const Wt::Dbo::ptr<User>& user, 
        const std::string& method) const override;
    void processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const override;
};
