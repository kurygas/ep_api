#include "user_resource.h"
#include "root_requirements.h"

void UserResource::processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id, 
    const std::string& method) const {
    const auto user = session.getById<User>(id);

    if (method == Str::auth) {
        response[Str::token] = user->getToken(request.body().at(Str::password));
    }
    else {
        throw NotFoundException("");
    }
}

void UserResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& ptr) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            ptr.modify()->setName(value);
        }
        else if (key == Str::surname) {
            ptr.modify()->setSurname(value);
        }
        else if (key == Str::tgUsername) {
            ptr.modify()->setTgUsername(value);
        }
        else if (key == Str::userType) {
            ptr.modify()->setUserType(JsonFunctions::parse<UserType>(value));
        }
        else if (key == Str::groupId) {
            ptr.modify()->setGroup(session.getById<Group>(value));
        }
        else if (key == Str::workResultList) {
            ptr.modify()->setWorkResults(session.getByArray<WorkResult>(value));
        }
    }
}

void UserResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void UserResource::getIdRequirements(const HttpRequest& request, Session& session, int id) const {
    RootRequirements::requireAuth(request, session);
}

void UserResource::deleteRequirements(const HttpRequest& request, Session& session, int id) const {
    RootRequirements::requireTeacherRoots(request, session);
}
