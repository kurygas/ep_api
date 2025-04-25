#include "group.h"

Group::Group(const Wt::WString& groupName) {
    setName(groupName);
}

void Group::setName(const Wt::WString& groupName) {
    if (groupName.empty()) {
        throw std::runtime_error("Invalid group name");
    }

    groupName_ = groupName;
}

const Wt::WString& Group::getGroupName() const {
    return groupName_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<User>>& Group::getUsers() const {
    return users_;
}
