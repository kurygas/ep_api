#include "user.h"
#include "session.h"
#include "validator.h"
#include "group.h"
#include "work_result.h"
#include "random_functions.h"
#include "str.h"

#include <Wt/Auth/HashFunction.h>

std::unique_ptr<User> User::createAdmin() {
    auto user = std::make_unique<User>();
    user->tgId_ = "admin";
    user->tgUsername_ = "admin";
    user->setPassword("adminadmin");
    user->userType_ = UserType::Admin;
    return std::move(user);
}

User::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::userType] = static_cast<int>(getUserType());
    json[Str::name] = getName();
    json[Str::surname] = getSurname();
    json[Str::tgId] = getTgId();
    json[Str::tgUsername] = getTgUsername();
    json[Str::groupId] = getGroup().id();
    json[Str::workResultList] = JsonFunctions::getAllId(getWorkResults());
    return json;
}

User::User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& name, const Wt::WString& surname, 
    const Wt::WString& password)
: userType_(UserType::Student)
, tokenTimeLimit_(Wt::WDateTime::currentDateTime())
, salt_(Random::generateRandomString(16)) {
    setTgId(tgId);
    setTgUsername(tgUsername);
    setName(name);
    setSurname(surname);
    setPassword(password);
}

User::User(const Wt::Json::Object& json)
: User(json.at(Str::tgId), json.at(Str::tgUsername), json.at(Str::name), json.at(Str::surname), json.at(Str::password)) {}

bool User::isCorrect(const Wt::WString& password) const {
    return !salt_.empty() && Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_) == passwordHash_;
}

void User::setPassword(const Wt::WString& password) {
    if (!Validator::isPasswordValid(password)) {
        throw BadRequestException("Invalid password for User");
    }

    passwordHash_ = Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_);
}

void User::setName(const Wt::WString& firstName) {
    if (firstName.empty() || !Validator::isRussianString(firstName)) {
        throw BadRequestException("Invalid name for User");
    }

    name_ = firstName;
}

void User::setSurname(const Wt::WString& secondName) {
    if (secondName.empty() || !Validator::isRussianString(secondName)) {
        throw BadRequestException("Invalid surname for User");
    }

    surname_ = secondName;
}

void User::setTgId(const Wt::WString& tgId) {
    if (tgId.empty()) {
        throw BadRequestException("Invalid tg id for User");
    }

    tgId_ = tgId;
}

void User::setTgUsername(const Wt::WString& tgUsername) {
    if (tgUsername.empty() || tgUsername.toUTF8().front() != '@') {
        throw BadRequestException("Invalid tg username for User");
    }

    tgUsername_ = tgUsername;
}

void User::setUserType(const UserType userType) {
    if (userType == UserType::Admin) {
        throw BadRequestException("Invalid user type for User");
    }
    userType_ = userType;
}

void User::setGroup(const Wt::Dbo::ptr<Group>& group) {
    group_ = group;
}

void User::setToken(const Wt::WString& token) {
    if (token.toUTF8().size() != 16) {
        throw ServerException("Invalid token");
    }
    
    tokenTimeLimit_ = Wt::WDateTime::currentDateTime().addDays(14);
    token_ = token;
}

void User::setWorkResults(const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& workResults) {
    workResults_ = workResults;
}

const Wt::WString& User::getToken(const Wt::WString& password) const {
    if (!isCorrect(password)) {
        throw ForbiddenException("Invalid password");
    }

    return token_;
}

UserType User::getUserType() const {
    return userType_;
}

const Wt::WString& User::getName() const {
    return name_;
}

const Wt::WString& User::getSurname() const {
    return surname_;
}

const Wt::WString& User::getTgUsername() const {
    return tgUsername_;
}

const Wt::WString& User::getTgId() const {
    return tgId_;
}

const Wt::Dbo::ptr<Group>& User::getGroup() const {
    return group_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& User::getWorkResults() const {
    return workResults_;
}

const Wt::WDateTime& User::getTokenTimeLimit() const {
    return tokenTimeLimit_;
}

std::string User::getListName() {
    return Str::userList;
}
