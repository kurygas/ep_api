#pragma once

#include "resource.h"

class TokenResource : public Resource {
protected:
    void processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const override;
};