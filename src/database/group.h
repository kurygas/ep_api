#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"

class User;
class Semester;

class Group {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, cfGroupCode_, Str::cfGroupCode);
        Wt::Dbo::field(a, cfMaxPoint_, Str::cfMaxPoint);
        Wt::Dbo::field(a, atcMaxPoint_, Str::atcMaxPoint);
        Wt::Dbo::hasMany(a, users_, Wt::Dbo::ManyToOne, "group");
        Wt::Dbo::hasMany(a, semesters_, Wt::Dbo::ManyToOne, "group");
    }

    operator Wt::Json::Object() const;

    Group() = default;
    explicit Group(const Wt::WString& name);

    void setName(const Wt::WString& name);
    void setCfGroupCode(const Wt::WString& cfGroupCode);
    void setCfMaxPoint(int cfMaxPoint);
    void setAtcMaxPoint(int atcMaxPoint);

    const Wt::WString& getName() const;
    const Wt::WString& getCfGroupCode() const;
    int getCfMaxPoint() const;
    int getAtcMaxPoint() const;
    const List<User>& getUsers() const;
    const List<Semester>& getSemesters() const;

private:
    Wt::WString name_;
    Wt::WString cfGroupCode_;
    int cfMaxPoint_ = 0;
    int atcMaxPoint_ = 0;

    List<User> users_;
    List<Semester> semesters_;
};
