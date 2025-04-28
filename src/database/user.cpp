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

User::User(const Wt::WString& tgId, const Wt::WString& tgUsername, const Wt::WString& password, const Wt::WString& firstName, 
    const Wt::WString& secondName, const Wt::WString& email, const Wt::WString& token)
: userType_(UserType::Student)
, tokenTimeLimit_(Wt::WDateTime::currentDateTime())
, salt_(Random::generateRandomString(16)) {
    setTgId(tgId);
    setTgUsername(tgUsername);
    setPassword(password);
    setName(firstName);
    setSurname(secondName);
    setEmail(email);
}

bool User::isCorrect(const Wt::WString& password) const {
    return Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_) == passwordHash_;
}

void User::setPassword(const Wt::WString& password) {
    if (!isPasswordValid(password)) {
        throw BadRequestException("Invalid password for User");
    }

    passwordHash_ = Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_);
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
        throw BadRequestException("Invalid tgUsername for User");
    }

    tgUsername_ = tgUsername;
}

void User::setUserType(const UserType userType) {
    userType_ = userType;
}

void User::setEmail(const Wt::WString& email) {
    if (email.empty()) {
        throw BadRequestException("Invalid email for User");
    }

    email_ = email;
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

const Wt::WString& User::getToken() {
    return token_;
}

User::UserType User::getUserType() const {
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

const Wt::WString& User::getEmail() const {
    return email_;
}
