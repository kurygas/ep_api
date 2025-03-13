#pragma once

#include <Wt/Dbo/Session.h>

class User;
class Group;
class JsonObject;

class Session : public Wt::Dbo::Session {
public:
    Session();
    void configureDatabase();

    static std::string generateRandomString(int length);

    void addUser(const std::string& tgId, const std::string& tgUsername, const std::string& password, const std::string& firstName, 
        const std::string& secondName, const std::string& email);
    void addGroup(const std::string& groupName);

    Wt::Dbo::ptr<User> getUserByTgId(const std::string& tgId);
    Wt::Dbo::ptr<User> getUser(const JsonObject& requestContent);
    Wt::Dbo::ptr<User> getTarget(const JsonObject& requestContent);
    Wt::Dbo::ptr<Group> getGroupByName(const std::string& groupName);
};