#include "user_resource.h"
#include "root_requirements.h"

#include <ctime>

void UserResource::processPatch(const HttpRequest& request, Session& session, const Ptr<User>& user) const {
    for (const auto& [key, value] : request.body()) {
        if (key == Str::name) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setName(static_cast<std::string>(value));
        }
        else if (key == Str::surname) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setSurname(static_cast<std::string>(value));
        }
        else if (key == Str::tgUsername) {
            RootRequirements::requireAuthId(request, session, user);
            user.modify()->setTgUsername(static_cast<std::string>(value));
        }
        else if (key == Str::userType) {
            RootRequirements::requireTeacherRoots(request, session);

            if (user->getUserType() != UserType::Student || JsonFunctions::parse<UserType>(value) != UserType::Teacher) {
                throw ForbiddenException("No roots");
            }

            user.modify()->setUserType(UserType::Teacher);
        }
        else if (key == Str::groupId) {
            RootRequirements::requireTeacherRoots(request, session);
            user.modify()->setGroup(session.load<Group>(value));
        }
        else if(key == Str::cfName) {
            user.modify()->setCfName(static_cast<std::string>(value));
        }
        else if (key == Str::atcName) {
            user.modify()->setAtcName(static_cast<std::string>(value));
        }
    }
}

void UserResource::getRequirements(const HttpRequest& request, Session& session) const {
    RootRequirements::requireAuth(request);
}

void UserResource::getIdRequirements(const HttpRequest& request, Session& session, const Ptr<User>& user) const {
    RootRequirements::requireAuth(request);
}

void UserResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<User>& user) const {
    RootRequirements::requireTeacherRoots(request, session);
}
