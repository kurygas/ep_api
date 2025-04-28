#include "group.h"
#include "user.h"
#include "work.h"
#include "http_exceptions.h"

Group::Group(const Wt::WString& name) {
    setName(name);
}

void Group::setName(const Wt::WString& name) {
    if (name.empty()) {
        throw BadRequestException("Invalid name for Group");
    }

    name_ = name;
}

const Wt::WString& Group::getName() const {
    return name_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<User>>& Group::getUsers() const {
    return users_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& Group::getWorks() const {
    return works_;
}
