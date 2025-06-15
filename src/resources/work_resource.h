#pragma once

#include "resource.h"
#include "work.h"

class WorkResource : public Resource<Work> {
protected:
    void processPatch(const HttpRequest& request, Session& session, const Ptr<Work>& work) const override;
    void processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
        const Ptr<Work>& work, const std::string& method) const override;

    void getRequirements(const HttpRequest& request, Session& session) const override;
    void postRequirements(const HttpRequest& request, Session& session) const override;
    void getIdRequirements(const HttpRequest& request, Session& session, const Ptr<Work>& work) const override;
    void deleteRequirements(const HttpRequest& request, Session& session, const Ptr<Work>& work) const override;
};
