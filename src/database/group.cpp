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

void Group::setCfMaxPoint(const int cfMaxPoint) {
    if (cfMaxPoint < 0) {
        throw BadRequestException("Invalid cf_max_point for Group");
    }

    cfMaxPoint_ = cfMaxPoint;
}

void Group::setAtcMaxPoint(const int atcMaxPoint) {
    if (atcMaxPoint < 0) {
        throw BadRequestException("Invalid atc_max_point");
    }

    atcMaxPoint_ = atcMaxPoint;
}

const Wt::WString& Group::getName() const {
    return name_;
}

const Wt::WString& Group::getCfGroupCode() const {
    return cfGroupCode_;
}

int Group::getCfMaxPoint() const {
    return cfMaxPoint_;
}

int Group::getAtcMaxPoint() const {
    return atcMaxPoint_;
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
    json[Str::userList] = JsonFunctions::getIdArray(getUsers());
    json[Str::semesterList] = JsonFunctions::getIdArray(getSemesters());
    json[Str::cfGroupCode] = getCfGroupCode();
    json[Str::cfMaxPoint] = getCfMaxPoint();
    json[Str::atcMaxPoint] = getAtcMaxPoint();

    return json;
}
