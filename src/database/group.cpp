#include "group.h"
#include "user.h"
#include "work.h"

Group::Group(const Wt::WString& groupName) {
    setName(groupName);
}

void Group::setName(const Wt::WString& groupName) {
    if (groupName.empty()) {
        throw std::runtime_error("Invalid name for Group");
    }

    groupName_ = groupName;
}

const Wt::WString& Group::getGroupName() const {
    return groupName_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<User>>& Group::getUsers() const {
    return users_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& Group::getWorks() const {
    return works_;
}
