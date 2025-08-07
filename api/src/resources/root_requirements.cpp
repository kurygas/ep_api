#include "root_requirements.h"
#include "request.h"
#include "session.h"
#include "user.h"

RootRequirements::AuthData RootRequirements::verify(const HttpRequest& request) {
    const auto decodedToken = jwt::decode(request.token());
    const auto tokenPayload = decodedToken.get_payload_json();
    AuthData authData;

    if (tokenPayload.contains("tg_id")) {
        authData.dataType = "tg_id";
    }
    else if (tokenPayload.contains("admin_name")) {
        authData.dataType = "admin_name";
    }
    else {
        throw AuthException("Invalid token");
    }

    authData.data = tokenPayload.at(authData.dataType).to_str();
    
    jwt::verify()
        .with_issuer("auth_service")
        .with_type("access")
        .with_claim(authData.dataType, jwt::claim(authData.data))
        .allow_algorithm(jwt::algorithm::hs256{Str::authSecret})
        .verify(decodedToken);

    return authData;
}

void RootRequirements::requireAuth(const HttpRequest& request) {
    verify(request);
}

void RootRequirements::requireTeacherRoots(const HttpRequest& request, Session& session) {
    const auto authData = verify(request);

    if (authData.dataType == "admin_name") {
        return;
    }

    if (session.getByTgId<User>(std::stoll(authData.data))->getUserType() == UserType::Student) {
        throw ForbiddenException("Not teacher");
    }
}

void RootRequirements::requireAdminRoots(const HttpRequest& request) {
    if (verify(request).dataType == "tg_id") {
        throw ForbiddenException("Not admin");
    }
}

void RootRequirements::requireAuthId(const HttpRequest& request, Session& session, const Ptr<User>& user) {
    const auto authData = verify(request);

    if (authData.dataType == "admin_name") {
        return;
    }

    const auto caller = session.getByTgId<User>(std::stoll(verify(request).data));

    if (caller != user && caller->getUserType() == UserType::Student) {
        throw ForbiddenException("Don't have access to this user");
    }
}
