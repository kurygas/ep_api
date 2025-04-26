#pragma once

#include "resource.h"

namespace GroupResources {
    class Create : public AdminResource {
    protected:
        void processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const override;
    };
}
