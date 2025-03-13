#include "user_resources.h"
#include "session.h"
#include "user.h"
#include "json.h"

#include <unordered_set>

void RegistrationResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    session.addUser(requestContent.getString("tg_id"), requestContent.getString("tg_username"), requestContent.getString("password"), 
        requestContent.getString("first_name"), requestContent.getString("second_name"), requestContent.getString("email"));
}

void AuthResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    const auto user = session.getUser(requestContent);
    user.modify()->setTgUsername(requestContent.getString("tg_username"));
    
    if (!user->passwordIsValid(requestContent.getString("password"))) {
        throw std::runtime_error("Invalid password");
    }

    responseContent.put("token", user.modify()->getToken());
}

void UpdateProfileResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    const auto user = session.getUser(requestContent);

    for (const auto& [key, value] : requestContent) {
        if (key == "tg_username") {
            user.modify()->setTgUsername(value);
        }
        else if (key == "first_name") {
            user.modify()->setFirstName(value);
        }
        else if (key == "second_name") {
            user.modify()->setSecondName(value);
        }
    }
}

void MakeTeacherResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    session.getTarget(requestContent).modify()->setUserType(User::UserType::Teacher);
}

void GetUserResource::processGet(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    const auto target = session.getTarget(requestContent);
    responseContent.put("tg_id", target->getTgId());
    responseContent.put("tg_username", target->getTgUsername());
    responseContent.put("first_name", target->getFirstName());
    responseContent.put("second_name", target->getSecondName());
    responseContent.put("user_type", User::userTypeToString(target->getUserType()));
}

void DeleteUserResource::processPost(const JsonObject& requestContent, JsonObject& responseContent, Session& session) const {
    session.getTarget(requestContent).remove();
}