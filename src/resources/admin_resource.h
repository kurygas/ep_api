#pragma once

#include "token_resource.h"

class AdminResource : public TokenResource {
protected:
    void processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const override;
};