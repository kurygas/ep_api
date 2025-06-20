#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"

class User;
class Work;

class Group {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::hasMany(a, users_, Wt::Dbo::ManyToOne, "group");
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToOne, "group");
    }

    operator Wt::Json::Object() const;

    Group() = default;
    explicit Group(const Wt::WString& name);

    void setName(const Wt::WString& name);

    const Wt::WString& getName() const;
    const List<User>& getUsers() const;
    const List<Work>& getWorks() const;

private:
    Wt::WString name_;

    List<User> users_;
    List<Work> works_;
};
