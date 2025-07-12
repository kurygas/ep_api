#include "group.h"
#include "user.h"
#include "work.h"
#include "str.h"
#include "http_exceptions.h"
#include "types.h"
#include "semester.h"
#include "validator.h"

Group::Group(const Wt::WString& name) {
    setName(name);
}

void Group::setName(const Wt::WString& name) {
    if (name.empty()) {
        throw BadRequestException("Invalid name for Group");
    }

    name_ = name;
}

void Group::setCfGroupCode(const Wt::WString& cfGroupCode) {
    cfGroupCode_ = cfGroupCode;
}

const Wt::WString& Group::getName() const {
    return name_;
}

const Wt::WString& Group::getCfGroupCode() const {
    return cfGroupCode_;
}

const List<User>& Group::getUsers() const {
    return users_;
}

const List<Semester>& Group::getSemesters() const {
    return semesters_;
}

Group::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();

    if (!getUsers().empty()) {
        json[Str::userList] = JsonFunctions::getIdArray(getUsers());
    }

    if (!getSemesters().empty()) {
        json[Str::semesterList] = JsonFunctions::getIdArray(getSemesters());
    }

    if (!getCfGroupCode().empty()) {
        json[Str::cfGroupCode] = getCfGroupCode();
    }
    
    return json;
}
