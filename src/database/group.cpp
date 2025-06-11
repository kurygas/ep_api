#include "group.h"
#include "user.h"
#include "work.h"
#include "str.h"
#include "http_exceptions.h"

Group::Group(const Wt::WString& name) {
    setName(name);
}

Group::Group(const Wt::Json::Object& json)
: Group(static_cast<Wt::WString>(json.at(Str::name))) {}

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

Group::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();
    json[Str::userList] = JsonFunctions::getIdArray(getUsers());
    json[Str::workList] = JsonFunctions::getIdArray(getWorks());
    return json;
}
