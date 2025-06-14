#pragma once

#include <Wt/Dbo/ptr.h>

class HttpRequest;
class Session;
class User;

namespace RootRequirements {
    void requireAuth(const HttpRequest& request, Session& session);
    void requireTeacherRoots(const HttpRequest& request, Session& session);
    void requireAdminRoots(const HttpRequest& request, Session& session);
    void requireAuthId(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user);
}
