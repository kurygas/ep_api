#include "token_resource.h"
#include "session.h"
#include "request.h"

void TokenResource::processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const {
    if (!session.tokenExists(request.getToken())) {
        throw std::runtime_error("Token does not exist");
    }

    Resource::processRequest(request, responseContent, session);
}