#include "group.h"
#include "user.h"
#include "work.h"
#include "str.h"
#include "http_exceptions.h"
#include "types.h"

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

const List<User>& Group::getUsers() const {
    return users_;
}

const List<Work>& Group::getWorks() const {
    return works_;
}

Group::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();
    json[Str::userList] = JsonFunctions::getIdArray(getUsers());
    json[Str::workList] = JsonFunctions::getIdArray(getWorks());
    return json;
}
