#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"

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
    static const std::string& getListName();

    Group() = default;
    explicit Group(const Wt::WString& name);

    void setName(const Wt::WString& name);

    const Wt::WString& getName() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<User>>& getUsers() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& getWorks() const;

private:
    Wt::WString name_;

    Wt::Dbo::collection<Wt::Dbo::ptr<User>> users_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Work>> works_;
};
