#pragma once

#include "resource.h"
#include "semester.h"

class SemesterResource : public Resource<Semester> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Semester>& semester) const override;
    Ptr<Semester> createObject(const Wt::Json::Object& json, Session& session) const override;
    void sendUpdatedInfo(const Ptr<Semester>& semester) const override;
    void sendDeletedInfo(const Ptr<Semester>& semester) const override;
};
