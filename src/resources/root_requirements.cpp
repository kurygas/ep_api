#include "root_requirements.h"
#include "request.h"
#include "session.h"
#include "user.h"

void RootRequirements::requireAuth(const HttpRequest& request, Session& session) {
    if (!session.exist(&Session::getByToken<User>, request.token())) {
        throw AuthException("");
    }
}

void RootRequirements::requireTeacherRoots(const HttpRequest& request, Session& session) {
    if (session.getByToken<User>(request.token())->getUserType() == UserType::Student) {
        throw ForbiddenException("Not teacher");
    }
}

void RootRequirements::requireAdminRoots(const HttpRequest& request, Session& session) {
    if (session.getByToken<User>(request.token())->getUserType() != UserType::Admin) {
        throw ForbiddenException("Not admin");
    }
}

void RootRequirements::requireAuthId(const HttpRequest& request, Session& session, const int id) {
    const auto caller = session.getByToken<User>(request.token());

    if (caller.id() != id && caller->getUserType() != UserType::Admin) {
        throw ForbiddenException("Don't have access to this user");
    }
}
