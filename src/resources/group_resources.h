#pragma once

#include "resource.h"

namespace GroupResources {
    class CreateGroupResource : public AdminResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };

    class AddUserResource : public AdminResource {
    protected:
        void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
    };
}