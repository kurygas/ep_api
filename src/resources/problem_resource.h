#pragma once

#include "resource.h"
#include "problem.h"

class ProblemResource : public Resource<Problem> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& group) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& problem) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Problem>& problem) const override;
};
