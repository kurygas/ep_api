#include "group.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "user.h"
#include "semester.h"

Group::Group(std::string name) {
    setName(std::move(name));
}

void Group::setName(std::string name) {
    if (name.empty()) {
        throw BadRequestException("Invalid name for Group");
    }

    name_ = std::move(name);
}

void Group::setCfGroupCode(std::string cfGroupCode) {
    cfGroupCode_ = std::move(cfGroupCode);
}

const std::string& Group::getName() const {
    return name_;
}

const std::string& Group::getCfGroupCode() const {
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
    json[Str::name] = getName().c_str();

    if (!getUsers().empty()) {
        json[Str::userList] = JsonFunctions::getIdArray(getUsers());
    }

    if (!getSemesters().empty()) {
        json[Str::semesterList] = JsonFunctions::getIdArray(getSemesters());
    }

    if (!getCfGroupCode().empty()) {
        json[Str::cfGroupCode] = getCfGroupCode().c_str();
    }
    
    return json;
}

