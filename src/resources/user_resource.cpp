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
    checkAuth(request, session);
    Wt::Json::Array array;

    for (const auto& user : session.getAll<User>()) {
        array.emplace_back(getJson(user));
    }

    response[Str::userList] = array;
}

void UserResource::processPost(const HttpRequest& request, Wt::Json::Object& response, Session& session, Wt::Dbo::Transaction& tr) const {
    try {
        const auto user = session.addUser(
            request.body().at(Str::tgId), 
            request.body().at(Str::tgUsername), 
            request.body().at(Str::name), 
            request.body().at(Str::surname) 
        );
        response = getJson(user);
        tr.commit();
        response[Str::userId] = user.id();
    }
    catch (...) {
        throw BadRequestException("Does not contains necessary fields");
    }
}

void UserResource::processGetId(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    checkAuth(request, session);
    response = getJson(session.getById<User>(id));
}

void UserResource::processDelete(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    checkTeacher(request, session);
    auto user = session.getById<User>(id);
    response = getJson(user);
    user.remove();
}

void UserResource::processPatch(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id) const {
    const auto user = session.getById<User>(id);

    for (const auto& [key, value] : request.body()) {
        if (key == Str::tgUsername) {
            checkAuthId(request, session, id);
            user.modify()->setTgUsername(value);
        }
        else if (key == Str::name) {
            checkAuthId(request, session, id);
            user.modify()->setName(value);
        }
        else if (key == Str::surname) {
            checkAuthId(request, session, id);
            user.modify()->setSurname(value);
        }
        else if (key == Str::password) {
            checkAuthId(request, session, id);
            user.modify()->setPassword(value);
        }
        else if (key == Str::confirmCode) {
            checkAuthId(request, session, id);
            user.modify()->confirmPassword(value);
        }
        else if (key == Str::userType) {
            const auto caller = checkTeacher(request, session);
            const auto type = parse<UserType>(value);

            if (caller->getUserType() == UserType::Teacher && type == UserType::Student) {
                throw BadRequestException("");
            }

            user.modify()->setUserType(type);
        }
    }

    response = getJson(user);
}

void UserResource::processPostMethod(const HttpRequest& request, Wt::Json::Object& response, Session& session, const int id, 
    const std::string& method) const {
    const auto user = session.getById<User>(id);

    if (method == Str::auth) {
        checkAuthId(request, session, id);
        response[Str::token] = user->getToken(request.body().at(Str::password));
    }
    else {
        throw NotFoundException("");
    }
}

Wt::Json::Object UserResource::getJson(const Wt::Dbo::ptr<User>& user) const {
    Wt::Json::Object json;
    json[Str::userId] = user.id();
    json[Str::userType] = static_cast<int>(user->getUserType());
    json[Str::name] = user->getName();
    json[Str::surname] = user->getSurname();
    json[Str::tgId] = user->getTgId();
    json[Str::tgUsername] = user->getTgUsername();
    json[Str::groupId] = user->getGroup().id();
    json[Str::workResultList] = getAllId(user->getWorkResults());
    return json;
}
