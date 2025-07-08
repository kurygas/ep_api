#pragma once

#include "resource.h"
#include "semester_result.h"

class SemesterResultResource : public Resource<SemesterResult> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<SemesterResult>& semesterResult) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<SemesterResult>& semesterResult) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<SemesterResult>& semesterResult) const override;

    void prepare(Session& session, const Ptr<SemesterResult>& semesterResult) const override;
};
