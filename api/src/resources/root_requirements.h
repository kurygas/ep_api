#pragma once

#include <jwt-cpp/jwt.h>

#include "types.h"

class HttpRequest;
class Session;
class User;

namespace RootRequirements {
    struct AuthData {
        std::string dataType;
        std::string data;
    };

    AuthData verify(const HttpRequest& request);
    void requireAuth(const HttpRequest& request);
    void requireTeacherRoots(const HttpRequest& request, Session& session);
    void requireAdminRoots(const HttpRequest& request);
    void requireAuthId(const HttpRequest& request, Session& session, const Ptr<User>& user);
}
