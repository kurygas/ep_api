#pragma once

#include "resource.h"
#include "work_result.h"

class WorkResultResource : public Resource<WorkResult> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<WorkResult>& workResult) const override;
    void processGetMethod(const HttpRequest& request, Wt::Http::Response& response, Session& session, 
        const Ptr<WorkResult>& workResult, const std::string& method) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<WorkResult>& workResult) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<WorkResult>& workResult) const override;
    virtual Ptr<WorkResultResource> createObject(const Wt::Json::Object& json, Session& session) const override;
};
