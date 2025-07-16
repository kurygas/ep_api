#include "user.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "crypto.h"
#include "group.h"
#include "semester_result.h"
#include "point.h"
#include "work_result.h"

User::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::userType] = static_cast<int>(getUserType());
    json[Str::name] = getName().c_str();
    json[Str::surname] = getSurname().c_str();
    json[Str::tgId] = getTgId();
    json[Str::tgUsername] = getTgUsername().c_str();

    if (!getGroup()) {
        json[Str::groupId] = getGroup().id();
    }

    if (!getSemesterResults().empty()) {
        json[Str::semesterResultList] = JsonFunctions::getIdArray(getSemesterResults());
    }

    if (!getCfName().empty()) {
        json[Str::cfName] = getCfName().c_str();
    }

    if (!getAtcName().empty()) {
        json[Str::atcName] = getAtcName().c_str();
    }
    
    return json;
}

std::unique_ptr<User> User::createAdmin(std::string name) {
    auto user = std::make_unique<User>();
    user->tgUsername_ = std::move(name);
    user->userType_ = UserType::Admin;
    return std::move(user);
}

User::User(const int64_t tgId, std::string tgUsername, std::string name, std::string surname)
: userType_(UserType::Student)
, tokenTimeLimit_(Wt::WDateTime::currentDateTime()) {
    setTgId(tgId);
    setTgUsername(std::move(tgUsername));
    setName(std::move(name));
    setSurname(std::move(surname));
}

void User::setName(std::string name) {
    if (name.empty() || !Validator::isRussianString(name)) {
        throw BadRequestException("Invalid name for User");
    }

    name_ = std::move(name);
}

void User::setSurname(std::string surname) {
    if (surname.empty() || !Validator::isRussianString(surname)) {
        throw BadRequestException("Invalid surname for User");
    }

    surname_ = std::move(surname);
}

void User::setTgUsername(std::string tgUsername) {
    if (tgUsername.empty()) {
        throw BadRequestException("Invalid tg username for User");
    }

    tgUsername_ = std::move(tgUsername);
}

void User::setGroup(Ptr<Group> group) {
    group_ = std::move(group);
}

void User::setToken(std::string token) {
    if (token.size() != 16) {
        throw ServerException("Invalid token");
    }
    
    tokenTimeLimit_ = Wt::WDateTime::currentDateTime().addDays(14);
    token_ = std::move(token);
}

void User::setCfName(std::string cfName) {
    if (cfName.empty()) {
        throw BadRequestException("Invalid cf name");
    }

    cfName_ = std::move(cfName);
}

void User::setTgId(const int64_t tgId) {
    tgId_ = tgId;
}

void User::setUserType(const UserType userType) {
    if (userType == UserType::Admin) {
        throw BadRequestException("Invalid user type for User");
    }
    userType_ = userType;
}

void User::setAtcName(std::string atcName) {
    if (atcName.empty()) {
        throw BadRequestException("Invalid atc name");
    }

    atcName_ = std::move(atcName);
}

void User::setCfUpdated() {
    lastCfUpdate_ = Wt::WDateTime::currentDateTime();
}

void User::setAtcUpdated() {
    lastAtcUpdate_ = Wt::WDateTime::currentDateTime();
}

const std::string& User::getToken(const std::string& checkString, const std::string& hash) const {
    if (Crypto::hmacSha256(checkString, Crypto::sha256(Str::botToken)) != hash) {
        throw ForbiddenException("Incorrect auth data");
    }

    return token_;
}

UserType User::getUserType() const {
    return userType_;
}

const std::string& User::getName() const {
    return name_;
}

const std::string& User::getSurname() const {
    return surname_;
}

const std::string& User::getTgUsername() const {
    return tgUsername_;
}

int64_t User::getTgId() const {
    return tgId_;
}

const Ptr<Group>& User::getGroup() const {
    return group_;
}

const Wt::WDateTime& User::getTokenTimeLimit() const {
    return tokenTimeLimit_;
}

const std::string& User::getCfName() const {
    return cfName_;
}

const std::string& User::getAtcName() const {
    return atcName_;
}

const List<SemesterResult> User::getSemesterResults() const {
    return semesterResults_;
}

const Wt::WDateTime& User::getLastCfUpdate() const {
    return lastCfUpdate_;
}

const Wt::WDateTime& User::getLastAtcUpdate() const {
    return lastAtcUpdate_;
}
