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
    std::string generateToken();

    void addUser(const std::string& tgId, const std::string& tgUsername, const std::string& password, const std::string& firstName, 
        const std::string& secondName, const std::string& email);
    void addGroup(const std::string& groupName);

    Wt::Dbo::ptr<User> getUserByTgId(const std::string& tgId);
    Wt::Dbo::ptr<User> getUserByToken(const std::string& token);
    Wt::Dbo::ptr<Group> getGroupByGroupName(const std::string& groupName);
    Wt::Dbo::ptr<Group> getGroupByGroupId(int groupId);
    
    bool tgIdExists(const std::string& tgId);
    bool tokenExists(const std::string& token);
    bool groupNameExists(const std::string& groupName);

    Wt::Dbo::collection<Wt::Dbo::ptr<User>> getAllUsers();

private:
    template<typename F, typename... Args>
    bool exist(const F method, Args&&... args) {
        try {
            (this->*method)(std::forward<Args>(args)...);
        }
        catch (...) {
            return false;
        }

        return true;
    }

    void checkUser(const Wt::Dbo::ptr<User>& user) const;
    void checkGroup(const Wt::Dbo::ptr<Group>& group) const;
};