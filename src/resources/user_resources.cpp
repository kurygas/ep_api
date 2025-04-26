#include "user_resources.h"
#include "session.h"
#include "user.h"
#include "request.h"

using namespace UserResources;

void Create::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    const auto& body = request.getBody();
    session.addUser(body.at("tg_id"), body.at("tg_username"), body.at("password"), body.at("first_name"), body.at("second_name"), 
        body.at("email"));
}

void Auth::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    const auto& body = request.getBody();
    const auto user = session.getUserByTgId(body.at("tg_id"));
    user.modify()->setTgUsername(body.at("tg_username"));
    
    if (!user->passwordIsValid(body.at("password"))) {
        throw std::runtime_error("Invalid password");
    }

    response["token"] = user.modify()->getToken();
}

void MakeAdmin::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.getUserByTgId(request.getBody().at("tg_id")).modify()->setUserType(User::UserType::Teacher);
}

void Get::processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    const auto user = session.getUserByTgId(request.getParam("tg_id"));
    response["tg_id"] = user->getTgId();
    response["tg_username"] = user->getTgUsername();
    response["first_name"] = user->getFirstName();
    response["second_name"] = user->getSecondName();
    response["user_type"] = static_cast<int>(user->getUserType());
}

void Delete::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.getUserByTgId(request.getBody().at("tg_id")).remove();
}

void SetTgUsername::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.getUserByToken(request.getToken()).modify()->setTgUsername(request.getBody().at("tg_username"));
}

void SetFirstName::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.getUserByToken(request.getToken()).modify()->setFirstName(request.getBody().at("first_name"));
}

void SetSecondName::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.getUserByToken(request.getToken()).modify()->setSecondName(request.getBody().at("second_name"));
}

void GetAllId::processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    Wt::Json::Array array;

    for (const auto& user : session.getAll<User>()) {
        array.emplace_back(user->getTgId());
    }

    response["all_users"] = array;
}
