#include "user.h"
#include "session.h"
#include "checker.h"
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

UserType User::parse(const Wt::Json::Value& value) {
    return static_cast<UserType>(static_cast<int>(value));
}

User::User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& name, const Wt::WString& surname, 
    const Wt::WString& token)
: userType_(UserType::Student)
, tokenTimeLimit_(Wt::WDateTime::currentDateTime())
, confirmTimeLimit_(Wt::WDateTime::currentDateTime()) {
    setTgId(tgId);
    setTgUsername(tgUsername);
    setName(name);
    setSurname(surname);
}

bool User::isCorrect(const Wt::WString& password) const {
    return !salt_.empty() && Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_) == passwordHash_;
}

void User::setPassword(const Wt::WString& password) {
    if (!isPasswordValid(password)) {
        throw BadRequestException("Invalid password for User");
    }

    if (salt_.empty()) {
        salt_ = Random::generateRandomString(16);
    }

    comingPasswordHash_ = Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_);
    confirmCode_ = Random::generateRandomNumString(6);
    confirmTimeLimit_ = Wt::WDateTime::currentDateTime().addSecs(300);
}

void User::confirmPassword(const Wt::WString& confirmCode) {
    if (confirmCode != confirmCode_ || confirmTimeLimit_ < Wt::WDateTime::currentDateTime() || comingPasswordHash_.empty()) {
        throw ForbiddenException("Incorrect confirm code");
    }

    passwordHash_ = comingPasswordHash_;
    comingPasswordHash_.clear();
    confirmCode_.clear();
    confirmTimeLimit_ = Wt::WDateTime::currentDateTime();
}

void User::setName(const Wt::WString& firstName) {
    if (firstName.empty() || !isRussianString(firstName)) {
        throw BadRequestException("Invalid name for User");
    }

    name_ = firstName;
}

void User::setSurname(const Wt::WString& secondName) {
    if (secondName.empty() || !isRussianString(secondName)) {
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
    if (tgUsername.empty() || tgUsername.toUTF32().front() != '@') {
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

const Wt::WString& User::getToken(const Wt::WString& password) const {
    if (!isCorrect(password)) {
        throw ForbiddenException("Invalid password");
    }

    return token_;
}

UserType User::getUserType() const
{
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

const std::string& User::getConfirmCode() const {
    return confirmCode_;
}
