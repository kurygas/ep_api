#pragma once

#include "resource.h"

namespace GroupResources {
    class CreateGroupResource : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };

    class AddUserResource : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };
}