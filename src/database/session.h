#pragma once

#include <Wt/Dbo/Session.h>

class User;
class Group;

class Session : public Wt::Dbo::Session {
public:
    Session();
    void configureDatabase();

    Wt::WString generateToken();

    void addUser(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, const Wt::WString& firstName, 
        const Wt::WString& secondName, const Wt::WString& email);
    void addGroup(const Wt::WString& groupName);

    Wt::Dbo::ptr<User> getUserByTgId(const Wt::WString& tgId);
    Wt::Dbo::ptr<User> getUserByToken(const Wt::WString& token);
    Wt::Dbo::ptr<Group> getGroupByGroupName(const Wt::WString& groupName);
    Wt::Dbo::ptr<Group> getGroupByGroupId(int groupId);
    
    bool tgIdExists(const Wt::WString& tgId);
    bool tokenExists(const Wt::WString& token);
    bool groupNameExists(const Wt::WString& groupName);

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