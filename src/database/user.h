#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "json.h"

class Group;
class WorkResult;

enum class UserType {
    Student = 0,
    Teacher,
    Admin
};

class User {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, userType_, "user_type");
        Wt::Dbo::field(a, passwordHash_, "password_hash");
        Wt::Dbo::field(a, salt_, "salt");
        Wt::Dbo::field(a, name_, "name");
        Wt::Dbo::field(a, surname_, "surname");
        Wt::Dbo::field(a, tgId_, "tg_id");
        Wt::Dbo::field(a, tgUsername_, "tg_username");
        Wt::Dbo::field(a, token_, "token");
        Wt::Dbo::field(a, tokenTimeLimit_, "token_time_limit");
        Wt::Dbo::field(a, comingPasswordHash_, "coming_password_hash");
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "user");
    }

    static std::unique_ptr<User> createAdmin();
    static UserType parse(const Wt::Json::Value& value);

    User() = default;
    User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& name, const Wt::WString& surname, const Wt::WString& token);

    bool isCorrect(const Wt::WString& password) const;

    void setName(const Wt::WString& name);
    void setSurname(const Wt::WString& surname);
    void setTgUsername(const Wt::WString& tgUsername);
    void setUserType(UserType userType);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void setToken(const Wt::WString& token);
    void setPassword(const Wt::WString& password);
    void confirmPassword(const Wt::WString& confirmCode);

    const Wt::WString& getToken(const Wt::WString& password) const;
    UserType getUserType() const;
    const Wt::WString& getName() const;
    const Wt::WString& getSurname() const;
    const Wt::WString& getTgUsername() const;
    const Wt::WString& getTgId() const;
    const Wt::Dbo::ptr<Group>& getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;
    const Wt::WDateTime& getTokenTimeLimit() const;
    const std::string& getConfirmCode() const;

private:
    void setTgId(const Wt::WString& tgId);

    UserType userType_;
    Wt::WString name_;
    Wt::WString surname_;
    Wt::WString tgId_;
    Wt::WString tgUsername_;
    Wt::WString token_;
    Wt::WDateTime tokenTimeLimit_;

    std::string passwordHash_;
    std::string salt_;
    std::string comingPasswordHash_;
    std::string confirmCode_;
    Wt::WDateTime confirmTimeLimit_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
};
