#include "group.h"

std::unique_ptr<Group> Group::create(const std::string& groupName) {
    auto group = std::make_unique<Group>();
    group->setName(groupName);
    return std::move(group);
}

void Group::setName(const std::string& groupName)
{
    if (groupName.empty()) {
        std::runtime_error("Invalid group name");
    }

    groupName_ = groupName;
}

const std::string& Group::getGroupName() const {
    return groupName_;
}