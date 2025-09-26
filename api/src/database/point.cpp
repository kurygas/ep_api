#include "point.h"

Point::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::reason] = getReason().c_str();
    json[Str::amount] = getAmount();
    json[Str::semesterResultId] = getSemesterResult().id();
    return json;
}

Point::Point(std::string reason, int amount, Ptr<SemesterResult> semesterResult) {
    setReason(std::move(reason));
    setAmount(amount);
    setSemesterResult(std::move(semesterResult));
}

void Point::setReason(std::string reason) {
    if (reason.empty()) {
        throw BadRequestException("Invalid reason for Point");
    }

    reason_ = std::move(reason);
}

void Point::setSemesterResult(Ptr<SemesterResult> semesterResult) {
    if (!semesterResult) {
        throw BadRequestException("Invalid semester_result for Point");
    }

    semesterResult_ = std::move(semesterResult);
}

void Point::setAmount(const int amount) {
    if (amount <= 0) {
        throw BadRequestException("Invalid amount for Point");
    }

    amount_ = amount;
}

const std::string& Point::getReason() const {
    return reason_;
}

int Point::getAmount() const {
    return amount_;
}

const Ptr<SemesterResult>& Point::getSemesterResult() const {
    return semesterResult_;
}
