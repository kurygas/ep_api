#include "user.h"
#include "session.h"

#include <Wt/Auth/HashFunction.h>

std::unique_ptr<User> User::create(const std::string& tgId, const std::string& tgUsername, const std::string& password, 
    const std::string& firstName, const std::string& secondName, const std::string& email) {
    auto user = std::make_unique<User>();
    user->salt_ = Session::generateRandomString(16);
    user->setTgId(tgId);
    user->setTgUsername(tgUsername);
    user->setFirstName(firstName);
    user->setSecondName(secondName);
    user->setPassword(password);
    user->setEmail(email);
    return std::move(user);
}

bool User::isRussianString(const Wt::WString& str) {
    for (const auto& letter : str.toUTF16()) {
		if (!(0x0410 <= letter && letter <= 0x044F || letter == 0x0401 || letter == 0x0451)) {
			return false;
		}
	}

	return true;
}

std::string User::userTypeToString(const UserType userType) {
    if (userType == UserType::Student) {
        return "student";
    }
    else if (userType == UserType::Teacher) {
        return "teacher";
    }
    else if (userType == UserType::Admin) {
        return "admin";
    }
    else {
        throw std::runtime_error("Invalid user type");
    }
}

bool User::passwordIsValid(const std::string& password) const {
    return Wt::Auth::BCryptHashFunction().compute(password, salt_) == passwordHash_;
}

void User::setPassword(const std::string& password) {
    if (password.size() < 8) {
        throw std::runtime_error("Invalid password length");
    }

    passwordHash_ = Wt::Auth::BCryptHashFunction().compute(password, salt_);
}

void User::setFirstName(const Wt::WString& firstName) {
    if (firstName.empty() || !isRussianString(firstName)) {
        throw std::runtime_error("Invalid person name");
    }

    firstName_ = firstName;
}

void User::setSecondName(const Wt::WString& secondName) {
    if (secondName.empty() || !isRussianString(secondName)) {
        throw std::runtime_error("Invalid person name");
    }

    secondName_ = secondName;
}

void User::setTgId(const std::string& tgId) {
    if (tgId.empty()) {
        throw std::runtime_error("Invalid tg id");
    }

    tgId_ = tgId;
}

void User::setTgUsername(const std::string& tgUsername) {
    if (tgUsername.empty()) {
        throw std::runtime_error("Invalid tg username");
    }

    tgUsername_ = tgUsername;
}

void User::setEmail(const std::string& email) {
    if (email.empty()) {
        throw std::runtime_error("Invalid email");
    }

    email_ = email;
}

void User::setUserType(const UserType userType) {
    userType_ = userType;
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

void User::updateToken() {
    token_ = Session::generateRandomString(16);
    tokenTimeLimit_ = Wt::WDateTime::currentDateTime().addDays(14);
}