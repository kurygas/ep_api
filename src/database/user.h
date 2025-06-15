#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "json.h"
#include "str.h"

class Group;
class WorkResult;
class Point;

enum class UserType {
    Student = 0,
    Teacher,
    Admin
};

class User {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, userType_, Str::userType);
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, surname_, Str::surname);
        Wt::Dbo::field(a, tgId_, Str::tgId);
        Wt::Dbo::field(a, tgUsername_, Str::tgUsername);
        Wt::Dbo::field(a, token_, Str::token);
        Wt::Dbo::field(a, tokenTimeLimit_, "token_time_limit");
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "user");
        Wt::Dbo::hasMany(a, points_, Wt::Dbo::ManyToOne, "user");
    }

    static std::unique_ptr<User> createAdmin(const Wt::WString& name);
    operator Wt::Json::Object() const;
    static const std::string& getListName();

    User() = default;
    User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& name, const Wt::WString& surname);

    void setName(const Wt::WString& name);
    void setSurname(const Wt::WString& surname);
    void setTgUsername(const Wt::WString& tgUsername);
    void setUserType(UserType userType);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void setToken(const Wt::WString& token);

    const Wt::WString& getToken(const Wt::WString& checkString, const Wt::WString& hash) const;
    UserType getUserType() const;
    const Wt::WString& getName() const;
    const Wt::WString& getSurname() const;
    const Wt::WString& getTgUsername() const;
    const Wt::WString& getTgId() const;
    const Wt::Dbo::ptr<Group>& getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;
    const Wt::WDateTime& getTokenTimeLimit() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Point>>& getPoints() const;

private:
    void setTgId(const Wt::WString& tgId);

    UserType userType_;
    Wt::WString name_;
    Wt::WString surname_;
    Wt::WString tgId_;
    Wt::WString tgUsername_;
    Wt::WString token_;
    Wt::WDateTime tokenTimeLimit_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Point>> points_;
};
