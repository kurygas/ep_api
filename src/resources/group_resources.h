#pragma once

#include "resource.h"

class CreateGroupResource : public AdminResource {
protected:
    void processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const override;
};