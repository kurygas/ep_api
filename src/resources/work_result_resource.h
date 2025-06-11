#pragma once

#include "resource.h"
#include "work_result.h"

class WorkResultResource : public Resource<WorkResult> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<WorkResult>& workResult) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<WorkResult>& workResult) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<WorkResult>& workResult) const override;
};
