#include "point.h"
#include "http_exceptions.h"

Point::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::reason] = getReason();
    json[Str::amount] = getAmount();
    json[Str::userId] = getUser().id();
    return json;
}

Point::Point(const Wt::WString& reason, const int amount, const Ptr<User>& user) {
    setReason(reason);
    setAmount(amount);
    setUser(user);
}

void Point::setReason(const Wt::WString& reason) {
    if (reason.empty()) {
        throw BadRequestException("Empty reason");
    }

    reason_ = reason;
}

void Point::setAmount(const int amount) {
    if (amount <= 0) {
        throw BadRequestException("Amount must be positive");
    }

    amount_ = amount;
}

void Point::setUser(const Ptr<User>& user) {
    user_ = user;
}

const Wt::WString& Point::getReason() const {
    return reason_;
}

int Point::getAmount() const {
    return amount_;
}

const Ptr<User>& Point::getUser() const {
    return user_;
}

const std::string& Point::getListName() {
    return Str::pointList;
}
