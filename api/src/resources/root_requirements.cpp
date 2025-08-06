#include "root_requirements.h"
#include "request.h"
#include "session.h"
#include "user.h"

#include <jwt-cpp/jwt.h>

int64_t RootRequirements::verify(const HttpRequest& request) {
    const auto decode = jwt::decode(request.token());
    const auto verifier = jwt::verify()
        .with_issuer("ep_api")
        .with_type("access")
        .allow_algorithm(jwt::algorithm::hs256{Str::authSecret});
    verifier.verify(decode);
    return std::stoi(decode.get_payload_json().at("tg_id").to_str());
}

void RootRequirements::requireAuth(const HttpRequest& request, Session& session) {
    if (!session.exist(&Session::getByTgId<User>, verify(request))) {
        throw AuthException("Not authorized");
    }
}

void RootRequirements::requireTeacherRoots(const HttpRequest& request, Session& session) {
    if (session.getByTgId<User>(verify(request))->getUserType() == UserType::Student) {
        throw ForbiddenException("Not teacher");
    }
}

void RootRequirements::requireAdminRoots(const HttpRequest& request, Session& session) {
    if (session.getByTgId<User>(verify(request))->getUserType() != UserType::Admin) {
        throw ForbiddenException("Not admin");
    }
}

void RootRequirements::requireAuthId(const HttpRequest& request, Session& session, const Ptr<User>& user) {
    const auto caller = session.getByTgId<User>(verify(request));

    if (caller != user && caller->getUserType() == UserType::Student) {
        throw ForbiddenException("Don't have access to this user");
    }
}
