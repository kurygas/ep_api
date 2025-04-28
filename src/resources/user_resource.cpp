#include "user_resource.h"
#include "session.h"
#include "request.h"
#include "user.h"
#include "group.h"
#include "problem.h"
#include "work.h"
#include "work_result.h"
#include "str.h"

void UserResource::processGet(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    Wt::Json::Array array;

    for (const auto& user : session.getAll<User>()) {
        array.emplace_back(getJson(user));
    }

    response[Str::userList] = array;
}

void UserResource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session) const {
    session.addUser(request.body().at(Str::tgId), 
        request.body().at(Str::tgUsername), 
        request.body().at(Str::password), 
        request.body().at(Str::name), 
        request.body().at(Str::surname), 
        request.body().at(Str::email)
    );
}

void UserResource::processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    response = getJson(session.getById<User>(id));
}

void UserResource::processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    auto user = session.getById<User>(id);
    response = getJson(user);
    user.remove();
}

Wt::Json::Object UserResource::getJson(const Wt::Dbo::ptr<User>& user) const {
    Wt::Json::Object json;
    json[Str::userId] = user.id();
    json[Str::userType] = static_cast<int>(user->getUserType());
    json[Str::name] = user->getName();
    json[Str::surname] = user->getSurname();
    json[Str::tgId] = user->getTgId();
    json[Str::tgUsername] = user->getTgUsername();
    json[Str::email] = user->getEmail();
    json[Str::groupId] = user->getGroup().id();
    json[Str::workResultList] = getAllId(user->getWorkResults());
    return json;
}
