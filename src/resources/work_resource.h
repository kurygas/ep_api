#pragma once

#include "resource.h"
#include "work.h"

class WorkResource : public Resource<Work> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<Work>& group) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, int id) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, int id) const override;
};
