#include "user.h"
#include "session.h"
#include "checker.h"
#include "group.h"
#include "work_result.h"

#include <Wt/Auth/HashFunction.h>

std::unique_ptr<User> User::createAdmin() {
    auto user = std::make_unique<User>();
    user->tgId_ = "admin";
    user->tgUsername_ = "admin";
    user->setPassword("adminadmin");
    user->userType_ = UserType::Admin;
    return std::move(user);
}

User::User(const std::string& tgId, const std::string& tgUsername, const std::string& password, const Wt::WString& firstName, 
    const Wt::WString& secondName, const std::string& email)
: userType_(UserType::Student)
, tokenTimeLimit_(Wt::WDateTime::currentDateTime())
, salt_(Session::generateRandomString(16)) {
    setTgId(tgId);
    setTgUsername(tgUsername);
    setPassword(password);
    setFirstName(firstName);
    setSecondName(secondName);
    setEmail(email);
}

bool User::passwordIsValid(const std::string& password) const {
    return Wt::Auth::BCryptHashFunction().compute(password, salt_) == passwordHash_;
}

void User::setPassword(const std::string& password) {
    if (password.size() < 8) {
        throw std::runtime_error("Invalid password for User");
    }

    passwordHash_ = Wt::Auth::BCryptHashFunction().compute(password, salt_);
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

void User::setTgId(const std::string& tgId) {
    if (tgId.empty()) {
        throw std::runtime_error("Invalid tg id for User");
    }

    tgId_ = tgId;
}

void User::setTgUsername(const std::string& tgUsername) {
    if (tgUsername.empty() || tgUsername.front() != '@') {
        throw std::runtime_error("Invalid tgUsername for User");
    }

    tgUsername_ = tgUsername;
}

void User::setEmail(const std::string& email) {
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

const std::string& User::getToken() {
    if (Wt::WDateTime::currentDateTime() > tokenTimeLimit_) {
        updateToken();
    }

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

const std::string& User::getTgUsername() const {
    return tgUsername_;
}

const std::string& User::getTgId() const {
    return tgId_;
}

const Wt::Dbo::ptr<Group>& User::getGroup() const {
    return group_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& User::getWorkResults() const {
    return workResults_;
}

const std::string& User::getEmail() const {
    return email_;
}

void User::updateToken() {
    token_ = Session::generateRandomString(16);
    tokenTimeLimit_ = Wt::WDateTime::currentDateTime().addDays(14);
}