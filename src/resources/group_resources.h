#pragma once

#include "resource.h"

class CreateGroupResource : public AdminResource {
protected:
    void processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const override;
};