#pragma once

#include "resource.h"
#include "group.h"

class GroupResource : public Resource<Group> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Group>& group) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, int id) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, int id) const override;
};
