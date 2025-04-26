#include "session.h"
#include "user.h"
#include "group.h"
#include "problem.h"
#include "work_result.h"
#include "work.h"
#include "checker.h"

#include <Wt/Dbo/backend/Sqlite3.h>

Session::Session() {
    setConnection(std::make_unique<Wt::Dbo::backend::Sqlite3>("database.db"));
    mapClass<User>("users");
    mapClass<Group>("groups");
    mapClass<Problem>("problems");
    mapClass<WorkResult>("work_results");
    mapClass<Work>("works");
}

void Session::configureDatabase() {
    try {
        createTables();
    }
    catch (...) {}

    const Wt::Dbo::Transaction transaction(*this);
    
    if (!tgIdExists("admin")) {
        add(User::createAdmin());
    }
}

Wt::WString Session::generateToken() {
    Wt::WString token;

    do {
        token = generateRandomString(16);
    }
    while (tokenExists(token));

    return token;
}

Wt::Dbo::ptr<User> Session::getUserByTgId(const Wt::WString& tgId) {
    Wt::Dbo::ptr<User> user = find<User>().where("tg_id = ?").bind(tgId);
    checkUser(user);
    return user;
}

Wt::Dbo::ptr<User> Session::getUserByToken(const Wt::WString& token) {
    Wt::Dbo::ptr<User> user = find<User>().where("token = ?").bind(token);
    checkUser(user);
    return user;
}

Wt::Dbo::ptr<Group> Session::getGroupByGroupName(const Wt::WString& groupName) {
    Wt::Dbo::ptr<Group> group = find<Group>().where("group_name = ?").bind(groupName);
    checkGroup(group);
    return group;
}

Wt::Dbo::ptr<Group> Session::getGroupByGroupId(const int groupId) {
    Wt::Dbo::ptr<Group> group = find<Group>().where("id = ?").bind(groupId);
    checkGroup(group);
    return group;
}

bool Session::tgIdExists(const Wt::WString& tgId) {
    return exist(&Session::getUserByTgId, tgId);
}

bool Session::tokenExists(const Wt::WString& token) {
    return exist(&Session::getUserByToken, token);
}

bool Session::groupNameExists(const Wt::WString& groupName) {
    return exist(&Session::getGroupByGroupName, groupName);
}

Wt::Dbo::collection<Wt::Dbo::ptr<User>> Session::getAllUsers() {
    return find<User>().resultList();
}

void Session::checkUser(const Wt::Dbo::ptr<User>& user) const {
    if (!user) {
        throw std::runtime_error("User does not exist");
    }
}

void Session::checkGroup(const Wt::Dbo::ptr<Group>& group) const {
    if (!group) {
        throw std::runtime_error("Group does not exist");
    }
}

void Session::addUser(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, const Wt::WString& firstName, 
    const Wt::WString& secondName, const Wt::WString& email) {
    if (tgIdExists(tgId)) {
        throw std::runtime_error("User already exists");
    }

    add(std::make_unique<User>(tgId, tgUsername, password, firstName, secondName, email));
}

void Session::addGroup(const Wt::WString& groupName) {
    if (groupNameExists(groupName)) {
        throw std::runtime_error("Group already exists");
    }

    add(std::make_unique<Group>(groupName));
}