#pragma once

#include "resource.h"
#include "user.h"

class UserResource : public Resource<User> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<User>& user) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<User>& user) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<User>& user) const override;
};
