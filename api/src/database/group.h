#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

class User;
class Semester;

class Group {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, cfGroupCode_, Str::cfGroupCode);
        Wt::Dbo::hasMany(a, users_, Wt::Dbo::ManyToOne);
        Wt::Dbo::hasMany(a, semesters_, Wt::Dbo::ManyToOne);
    }

    operator Wt::Json::Object() const;

    Group() = default;
    explicit Group(std::string name);

    void setName(std::string name);
    void setCfGroupCode(std::string cfGroupCode);

    const std::string& getName() const;
    const std::string& getCfGroupCode() const;
    const List<User>& getUsers() const;
    const List<Semester>& getSemesters() const;

private:
    std::string name_;
    std::string cfGroupCode_;

    List<User> users_;
    List<Semester> semesters_;
};
