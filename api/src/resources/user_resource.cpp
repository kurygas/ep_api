#include "user_resource.h"
#include "root_requirements.h"
#include "message_queue.h"

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

void UserResource::deleteRequirements(const HttpRequest& request, Session& session, const Ptr<User>& user) const {
    RootRequirements::requireTeacherRoots(request, session);
}

Ptr<User> UserResource::createObject(const Wt::Json::Object& json, Session& session) const {
    const auto tgId = static_cast<int64_t>(json.at(Str::tgId));
    auto tgUsername = static_cast<std::string>(json.at(Str::tgUsername));
    auto name = static_cast<std::string>(json.at(Str::name));
    auto surname = static_cast<std::string>(json.at(Str::surname));

    if (session.exist(&Session::getByTgId<User>, tgId)) {
        throw UnprocessableEntityException("User already exists");
    }

    return session.add(std::make_unique<User>(tgId, std::move(tgUsername), std::move(name), std::move(surname)));
}

void UserResource::sendUpdatedInfo(const Ptr<User>& user) const {
    auto message = static_cast<Wt::Json::Object>(*user);
    message[Str::userId] = user.id();
    MessageQueue::getInstance().publish("algo_data", message);
}

void UserResource::sendDeletedInfo(const Ptr<User>& user) const {
    Wt::Json::Object message;
    message[Str::userId] = user.id();
    message["deleted"] = true;
    MessageQueue::getInstance().publish("algo_data", message);
}
