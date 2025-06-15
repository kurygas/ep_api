#include "user_resource.h"
#include "root_requirements.h"

#include <ctime>

void UserResource::processGetMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, 
    const Wt::Dbo::ptr<User>& user, const std::string& method) const {
    if (method == Str::token) {
        if (time(nullptr) - static_cast<int64_t>(request.body().at("auth_date")) >= 3600) {
            throw ForbiddenException("Too old data");
        }

        std::string checkString;

        for (const auto& [key, value] : request.body()) {
            checkString += key + '=' + static_cast<std::string>(value) + '\n';
        }

        checkString.pop_back();
        response[Str::token] = user->getToken(checkString, Str::botToken);
    }
    else {
        throw NotFoundException("");
    }
}

void UserResource::processPatch(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setName(value);
        }
        else if (key == Str::surname) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setSurname(value);
        }
        else if (key == Str::tgUsername) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setTgUsername(value);
        }
        else if (key == Str::userType) {
            RootRequirements::requireTeacherRoots(request, session);
            user.modify()->setUserType(JsonFunctions::parse<UserType>(value));
        }
        else if (key == Str::groupId) {
            RootRequirements::requireTeacherRoots(request, session);
            user.modify()->setGroup(session.getById<Group>(value));
        }
    }
}

void UserResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request, session);
}

void UserResource::getIdRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const {
    RootRequirements::requireAuth(request, session);
}

void UserResource::deleteRequirements(const HttpRequest& request, Session& session, const Wt::Dbo::ptr<User>& user) const {
    RootRequirements::requireTeacherRoots(request, session);
}
