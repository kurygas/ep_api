#pragma once

class HttpRequest;
class Session;

namespace RootRequirements {
    void requireAuth(const HttpRequest& request, Session& session);
    void requireTeacherRoots(const HttpRequest& request, Session& session);
    void requireAdminRoots(const HttpRequest& request, Session& session);
    void requireAuthId(const HttpRequest& request, Session& session, int id);
}