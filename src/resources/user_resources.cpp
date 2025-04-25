#include "user_resources.h"
#include "session.h"
#include "user.h"
#include "json.h"

#include <unordered_set>

using namespace UserResources;

void RegistrationResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto requestContent = getBodyContent(request);
    session.addUser(requestContent.at("tg_id"), requestContent.at("tg_username"), requestContent.at("password"), 
        requestContent.at("first_name"), requestContent.at("second_name"), requestContent.at("email"));
}

void AuthResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto requestContent = getBodyContent(request);
    const auto user = session.getUserByTgId(requestContent.at("tg_id"));
    user.modify()->setTgUsername(requestContent.at("tg_username"));
    
    if (!user->passwordIsValid(requestContent.at("password"))) {
        throw std::runtime_error("Invalid password");
    }

    responseContent.putString("token", user.modify()->getToken());
}

void MakeTeacherResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByTgId(getBodyContent(request).at("tg_id")).modify()->setUserType(User::UserType::Teacher);
}

void GetUserInfoResource::processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    const auto user = session.getUserByTgId(getParameter(request, "tg_id"));
    responseContent.putString("tg_id", user->getTgId());
    responseContent.putString("tg_username", user->getTgUsername());
    responseContent.putString("first_name", user->getFirstName());
    responseContent.putString("second_name", user->getSecondName());
    responseContent.putInt("user_type", user->getUserType());
}

void DeleteUserResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByTgId(getBodyContent(request).at("tg_id")).remove();
}

void UpdateTgUsernameResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByToken(getToken(request)).modify()->setTgUsername(getBodyContent(request).at("tg_username"));
}

void UpdateFirstNameResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByToken(getToken(request)).modify()->setFirstName(getBodyContent(request).at("first_name"));
}

void UpdateSecondNameResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByToken(getToken(request)).modify()->setSecondName(getBodyContent(request).at("second_name"));
}

void UpdateEmailResource::processPost(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    session.getUserByToken(getToken(request)).modify()->setEmail(getBodyContent(request).at("email"));
}

void GetAllUsersResource::processGet(const Wt::Http::Request& request, JsonObject& responseContent, Session& session) const {
    Wt::Json::Array array;

    for (const auto& user : session.getAllUsers()) {
        array.emplace_back(user->getTgId());
    }

    responseContent["all_users"] = array;
}
