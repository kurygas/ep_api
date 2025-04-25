#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

class Group;

class User {
public:
    enum class UserType {
        Student,
        Teacher,
        Admin
    };

    static std::unique_ptr<User> createAdmin();

    User(const std::string& tgId, const std::string& tgUsername, const std::string& password, const Wt::WString& firstName, 
        const Wt::WString& secondName, const std::string& email);
    
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, userType_, "user_type");
        Wt::Dbo::field(a, passwordHash_, "password_hash");
        Wt::Dbo::field(a, salt_, "salt");
        Wt::Dbo::field(a, firstName_, "first_name");
        Wt::Dbo::field(a, secondName_, "second_name");
        Wt::Dbo::field(a, tgId_, "tg_id");
        Wt::Dbo::field(a, tgUsername_, "tg_username");
        Wt::Dbo::field(a, token_, "token");
        Wt::Dbo::field(a, tokenTimeLimit_, "token_time_limit");
        Wt::Dbo::field(a, email_, "email");
        Wt::Dbo::belongsTo(a, group_, "group");
    }

    bool passwordIsValid(const std::string& password) const;

    void setPassword(const std::string& password);
    void setFirstName(const Wt::WString& firstName);
    void setSecondName(const Wt::WString& secondName);
    void setTgId(const std::string& tgId);
    void setTgUsername(const std::string& tgUsername);
    void setEmail(const std::string& email);
    void setUserType(UserType userType);
    void setGroup(const Wt::Dbo::ptr<Group>& group);

    const std::string& getToken();
    UserType getUserType() const;
    const Wt::WString& getFirstName() const;
    const Wt::WString& getSecondName() const;
    const std::string& getTgUsername() const;
    const std::string& getTgId() const;
    const Wt::Dbo::ptr<Group>& getGroup() const;
    const std::string& getEmail() const;

private:
    void updateToken();

    UserType userType_;
    std::string passwordHash_;
    std::string salt_;
    Wt::WString firstName_;
    Wt::WString secondName_;
    std::string tgId_;
    std::string tgUsername_;
    std::string email_;
    std::string token_;
    Wt::WDateTime tokenTimeLimit_;

    Wt::Dbo::ptr<Group> group_;
};