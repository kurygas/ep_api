#pragma once

#include "resource.h"
#include "group.h"

class GroupResource : public Resource<Group> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<Group>& group) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Group>& group) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Group>& group) const override;
    Ptr<Group> createObject(const Wt::Json::Object& json, Session& session) const override;
    void sendUpdatedInfo(const Ptr<Group>& group) const override;
    void sendDeletedInfo(const Ptr<Group>& group) const override;
};
