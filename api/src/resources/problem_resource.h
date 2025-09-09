#pragma once

#include "resource.h"
#include "problem.h"

class ProblemResource : public Resource<Problem> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<Problem>& group) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Problem>& problem) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Problem>& problem) const override;
    Ptr<Problem> createObject(const Wt::Json::Object& json, Session& session) const override;
};
