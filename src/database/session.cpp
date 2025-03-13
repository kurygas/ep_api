#include "session.h"
#include "user.h"
#include "group.h"
#include "json.h"

#include <Wt/Dbo/backend/Sqlite3.h>
#include <random>

Session::Session() {
    setConnection(std::make_unique<Wt::Dbo::backend::Sqlite3>("database.db"));
    mapClass<User>("users");
    mapClass<Group>("groups");
}

void Session::configureDatabase() {
    try {
        createTables();
    }
    catch (...) {}
}

std::string Session::generateRandomString(const int length) {
    static const std::string alphanumCharacters("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::mt19937 generator((std::random_device())());
    std::uniform_int_distribution<int> distribution(0, alphanumCharacters.size());
    std::string randomString(16, '\0');

    for (auto& ch : randomString) {
        ch = alphanumCharacters[distribution(generator)];
    }

    return randomString;
}

Wt::Dbo::ptr<User> Session::getUserByTgId(const std::string& tgId) {
    return find<User>().where("tg_id = ?").bind(tgId);
}

Wt::Dbo::ptr<Group> Session::getGroupByName(const std::string& groupName) {
    return find<Group>().where("group_name = ?").bind(groupName);
}

void Session::addUser(const std::string& tgId, const std::string& tgUsername, const std::string& password, const std::string& firstName, 
    const std::string& secondName, const std::string& email) {
    if (getUserByTgId(tgId)) {
        throw std::runtime_error("User already exists");
    }

    add(User::create(tgId, tgUsername, password, firstName, secondName, email));
}

void Session::addGroup(const std::string& groupName) {
    if (getGroupByName(groupName)) {
        throw std::runtime_error("Group already exists");
    }

    add(Group::create(groupName));
}

Wt::Dbo::ptr<User> Session::getUser(const JsonObject& requestContent) {
    auto user = getUserByTgId(requestContent.getString("tg_id"));

    if (!user) {
        throw std::runtime_error("User does not exist");
    }

    return user;
}

Wt::Dbo::ptr<User> Session::getTarget(const JsonObject& requestContent) {
    auto target = getUserByTgId(requestContent.getString("target"));
    
    if (!target) {
        throw std::runtime_error("Target does not exist");
    }

    return target;
}