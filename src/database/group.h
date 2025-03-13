#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

class User;

class Group {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, groupName_, "group_name");
        Wt::Dbo::hasMany(a, users_, Wt::Dbo::RelationType::ManyToOne, "group");
    }

    static std::unique_ptr<Group> create(const std::string& groupName);
    void setName(const std::string& groupName);
    const std::string& getGroupName() const;

private:
    std::string groupName_;

    Wt::Dbo::collection<Wt::Dbo::ptr<User>> users_;
};