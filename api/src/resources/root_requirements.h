#pragma once

#include "types.h"

class HttpRequest;
class Session;
class User;

namespace RootRequirements {
    int64_t verify(const HttpRequest& request);
    void requireAuth(const HttpRequest& request, Session& session);
    void requireTeacherRoots(const HttpRequest& request, Session& session);
    void requireAdminRoots(const HttpRequest& request, Session& session);
    void requireAuthId(const HttpRequest& request, Session& session, const Ptr<User>& user);
}
