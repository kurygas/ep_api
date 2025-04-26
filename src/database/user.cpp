#include "user.h"
#include "session.h"
#include "checker.h"
#include "group.h"
#include "work_result.h"
#include "random_functions.h"

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
    setFirstName(firstName);
    setSecondName(secondName);
    setEmail(email);
}

bool User::passwordIsValid(const Wt::WString& password) const {
    return Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_.toUTF8()) == passwordHash_;
}

void User::setPassword(const Wt::WString& password) {
    if (password.toUTF8().size() < 8) {
        throw std::runtime_error("Invalid password for User");
    }

    passwordHash_ = Wt::Auth::BCryptHashFunction().compute(password.toUTF8(), salt_.toUTF8());
}

void User::setFirstName(const Wt::WString& firstName) {
    if (firstName.empty() || !isRussianString(firstName)) {
        throw std::runtime_error("Invalid first name for User");
    }

    firstName_ = firstName;
}

void User::setSecondName(const Wt::WString& secondName) {
    if (secondName.empty() || !isRussianString(secondName)) {
        throw std::runtime_error("Invalid second name for User");
    }

    secondName_ = secondName;
}

void User::setTgId(const Wt::WString& tgId) {
    if (tgId.empty()) {
        throw std::runtime_error("Invalid tg id for User");
    }

    tgId_ = tgId;
}

void User::setTgUsername(const Wt::WString& tgUsername) {
    if (tgUsername.empty() || tgUsername.toUTF32().front() != '@') {
        throw std::runtime_error("Invalid tgUsername for User");
    }

    tgUsername_ = tgUsername;
}

void User::setEmail(const Wt::WString& email) {
    if (email.empty()) {
        throw std::runtime_error("Invalid email for User");
    }

    email_ = email;
}

void User::setUserType(const UserType userType) {
    userType_ = userType;
}

void User::setGroup(const Wt::Dbo::ptr<Group>& group) {
    group_ = group;
}

void User::setToken(const Wt::WString& token) {
    if (token.toUTF8().size() != 16) {
        throw std::runtime_error("Invalid token");
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

const Wt::WString& User::getFirstName() const {
    return firstName_;
}

const Wt::WString& User::getSecondName() const {
    return secondName_;
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
