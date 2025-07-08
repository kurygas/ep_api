#include "point.h"
#include "http_exceptions.h"
#include "validator.h"

Point::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::reason] = getReason();
    json[Str::amount] = getAmount();
    json[Str::semesterResultId] = getSemesterResult().id();
    return json;
}

Point::Point(const Wt::WString& reason, const int amount, const Ptr<SemesterResult>& semesterResult) {
    setReason(reason);
    setAmount(amount);
    setSemesterResult(semesterResult);
}

void Point::setReason(const Wt::WString& reason) {
    if (reason.empty() ||
        reason == "cf_points" ||
        reason == "atc_points") {
        throw BadRequestException("Invalid reason for Point");
    }

    reason_ = reason;
}

void Point::setAmount(const int amount) {
    if (amount <= 0) {
        throw BadRequestException("Invalid amount for Point");
    }

    amount_ = amount;
}

void Point::setSemesterResult(const Ptr<SemesterResult>& semesterResult) {
    semesterResult_ = semesterResult;
}

const Wt::WString& Point::getReason() const {
    return reason_;
}

int Point::getAmount() const {
    return amount_;
}

const Ptr<SemesterResult>& Point::getSemesterResult() const {
    return semesterResult_;
}
