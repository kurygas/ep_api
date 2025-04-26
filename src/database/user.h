#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

class Group;
class WorkResult;

class User {
public:
    enum class UserType {
        Student = 0,
        Teacher,
        Admin
    };

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
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "user");
    }

    static std::unique_ptr<User> createAdmin();

    User() = default;
    User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, const Wt::WString& firstName, 
        const Wt::WString& secondName, const Wt::WString& email, const Wt::WString& token);

    bool passwordIsValid(const Wt::WString& password) const;

    void setPassword(const Wt::WString& password);
    void setFirstName(const Wt::WString& firstName);
    void setSecondName(const Wt::WString& secondName);
    void setTgUsername(const Wt::WString& tgUsername);
    void setUserType(UserType userType);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void setToken(const Wt::WString& token);

    const Wt::WString& getToken();
    UserType getUserType() const;
    const Wt::WString& getFirstName() const;
    const Wt::WString& getSecondName() const;
    const Wt::WString& getTgUsername() const;
    const Wt::WString& getTgId() const;
    const Wt::WString& getEmail() const;
    const Wt::Dbo::ptr<Group>& getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;
    const Wt::WDateTime& getTokenTimeLimit() const;

private:
    void setTgId(const Wt::WString& tgId);
    void setEmail(const Wt::WString& email);

    UserType userType_;
    Wt::WString passwordHash_;
    Wt::WString salt_;
    Wt::WString firstName_;
    Wt::WString secondName_;
    Wt::WString tgId_;
    Wt::WString tgUsername_;
    Wt::WString email_;
    Wt::WString token_;
    Wt::WDateTime tokenTimeLimit_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
};
