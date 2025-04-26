#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

class User;
class Work;

class Group {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, groupName_, "group_name");
        Wt::Dbo::hasMany(a, users_, Wt::Dbo::ManyToOne, "group");
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToOne, "group");
    }

    Group() = default;
    Group(const Wt::WString& groupName);

    void setName(const Wt::WString& groupName);

    const Wt::WString& getGroupName() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<User>>& getUsers() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& getWorks() const;

private:
    Wt::WString groupName_;

    Wt::Dbo::collection<Wt::Dbo::ptr<User>> users_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Work>> works_;
};
