#include "admin_resource.h"
#include "session.h"
#include "request.h"
#include "user.h"

void AdminResource::processRequest(const HttpRequest& request, JsonObject& responseContent, Session& session) const {
    if (session.getUserByToken(request.getToken())->getUserType() == User::UserType::Student) {
        throw std::runtime_error("No permission");
    }

    Resource::processRequest(request, responseContent, session);
}