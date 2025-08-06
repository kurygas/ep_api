#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/WString.h>
#include <Wt/WDateTime.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

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
        Wt::Dbo::field(a, lastCfUpdate_, "last_cf_update");
        Wt::Dbo::field(a, lastAtcUpdate_, "last_atc_update");
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, semesterResults_, Wt::Dbo::ManyToOne, "user");
    }

    operator Wt::Json::Object() const;

    static std::unique_ptr<User> createAdmin(std::string name);

    User() = default;
    User(int64_t tgId, std::string tgUsername, std::string name, std::string surname);

    void setName(std::string name);
    void setSurname(std::string surname);
    void setTgUsername(std::string tgUsername);
    void setGroup(Ptr<Group> group);
    void setCfName(std::string cfName);
    void setAtcName(std::string atcName);
    void setUserType(UserType userType);
    void setCfUpdated();
    void setAtcUpdated();
    void updateRefreshToken(const std::string& tgCheckString, const std::string& hash);

    const std::string& getAccessToken() const;
    const std::string& getRefreshToken() const;
    UserType getUserType() const;
    const std::string& getName() const;
    const std::string& getSurname() const;
    const std::string& getTgUsername() const;
    int64_t getTgId() const;
    const Ptr<Group>& getGroup() const;
    const Wt::WDateTime& getTokenTimeLimit() const;
    const std::string& getCfName() const;
    const std::string& getAtcName() const;
    const List<SemesterResult> getSemesterResults() const;
    const Wt::WDateTime& getLastCfUpdate() const;
    const Wt::WDateTime& getLastAtcUpdate() const;

private:
    void setTgId(int64_t tgId);

    UserType userType_;
    std::string name_;
    std::string surname_;
    int64_t tgId_;
    std::string tgUsername_;
    std::string refreshToken_;
    std::string cfName_;
    Wt::WDateTime lastCfUpdate_;
    std::string atcName_;
    Wt::WDateTime lastAtcUpdate_;

    Ptr<Group> group_;
    List<SemesterResult> semesterResults_;
};
