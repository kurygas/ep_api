#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "json.h"
#include "str.h"
#include "types.h"

class Group;
class SemesterResult;
class Point;
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
        Wt::Dbo::field(a, userType_, Str::userType);
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, surname_, Str::surname);
        Wt::Dbo::field(a, tgId_, Str::tgId);
        Wt::Dbo::field(a, tgUsername_, Str::tgUsername);
        Wt::Dbo::field(a, token_, Str::token);
        Wt::Dbo::field(a, tokenTimeLimit_, "token_time_limit");
        Wt::Dbo::field(a, cfName_, Str::cfName);
        Wt::Dbo::field(a, atcName_, Str::atcName);
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, semesterResults_, Wt::Dbo::ManyToOne, "user");
        Wt::Dbo::hasMany(a, points_, Wt::Dbo::ManyToOne, "user");
    }

    static std::unique_ptr<User> createAdmin(const Wt::WString& name);
    operator Wt::Json::Object() const;

    User() = default;
    User(int64_t tgId, const Wt::WString& tgUsername, const Wt::WString& name, const Wt::WString& surname);

    void setName(const Wt::WString& name);
    void setSurname(const Wt::WString& surname);
    void setTgUsername(const Wt::WString& tgUsername);
    void setUserType(UserType userType);
    void setGroup(const Ptr<Group>& group);
    void setToken(const Wt::WString& token);
    void setCfName(const Wt::WString& cfName);
    void setAtcName(const Wt::WString& atcName);

    const Wt::WString& getToken(const Wt::WString& checkString, const Wt::WString& hash) const;
    UserType getUserType() const;
    const Wt::WString& getName() const;
    const Wt::WString& getSurname() const;
    const Wt::WString& getTgUsername() const;
    int64_t getTgId() const;
    const Ptr<Group>& getGroup() const;
    const Wt::WDateTime& getTokenTimeLimit() const;
    const List<Point>& getPoints() const;
    const Wt::WString& getCfName() const;
    const Wt::WString& getAtcName() const;
    const List<SemesterResult> getSemesterResults() const;

private:
    void setTgId(int64_t tgId);

    UserType userType_;
    Wt::WString name_;
    Wt::WString surname_;
    int64_t tgId_;
    Wt::WString tgUsername_;
    Wt::WString token_;
    Wt::WDateTime tokenTimeLimit_;
    Wt::WString cfName_;
    Wt::WString atcName_;

    Ptr<Group> group_;
    List<SemesterResult> semesterResults_;
    List<Point> points_;
};
