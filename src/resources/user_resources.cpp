#include "user_resources.h"
#include "session.h"
#include "user.h"
#include "json.h"

#include <unordered_set>

void RegistrationResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto requestContent = getBodyContent(request);
    session.addUser(requestContent.getString("tg_id"), requestContent.getString("tg_username"), requestContent.getString("password"), 
        requestContent.getString("first_name"), requestContent.getString("second_name"), requestContent.getString("email"));
}

void AuthResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto requestContent = getBodyContent(request);
    const auto user = session.getUserByTgId(requestContent.getString("tg_id"));
    user.modify()->setTgUsername(requestContent.getString("tg_username"));
    
    if (!user->passwordIsValid(requestContent.getString("password"))) {
        throw std::runtime_error("Invalid password");
    }

    responseContent.putString("token", user.modify()->getToken());
}

void MakeTeacherResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByTgId(getBodyContent(request).getString("target")).modify()->setUserType(User::UserType::Teacher);
}

void GetUserResource::processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto target = session.getUserByTgId(getParameter(request, "target"));
    responseContent.putString("tg_id", target->getTgId());
    responseContent.putString("tg_username", target->getTgUsername());
    responseContent.putString("first_name", target->getFirstName());
    responseContent.putString("second_name", target->getSecondName());
    responseContent.putString("user_type", User::userTypeToString(target->getUserType()));
}

void DeleteUserResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByTgId(getBodyContent(request).getString("target")).remove();
}