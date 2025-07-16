#include "semester_result.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "crypto.h"
#include "semester.h"
#include "user.h"
#include "work_result.h"
#include "point.h"

SemesterResult::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::semesterId] = getSemester().id();
    json[Str::userId] = getSemester().id();

    if (!getWorkResults().empty()) {
        json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    }

    if (!getPoints().empty()) {
        json[Str::pointList] = JsonFunctions::getIdArray(getPoints());
    }
    
    return json;
}

SemesterResult::SemesterResult(Ptr<Semester> semester, Ptr<User> user) {
    setSemester(std::move(semester));
    setUser(std::move(user));
}

void SemesterResult::setSemester(Ptr<Semester> semester) {
    if (!semester || user_ && semester->getGroup() != user_->getGroup()) {
        throw BadRequestException("Invalid Semester for SemesterResult");
    }

    semester_ = std::move(semester);
}

void SemesterResult::setUser(Ptr<User> user) {
    if (!user || semester_ && semester_->getGroup() != user->getGroup()) {
        throw BadRequestException("Invalid User for SemesterResult");
    }

    user_ = std::move(user);
}

const Ptr<Semester>& SemesterResult::getSemester() const {
    return semester_;
}

const Ptr<User>& SemesterResult::getUser() const {
    return user_;
}

const List<WorkResult>& SemesterResult::getWorkResults() const {
    return workResults_;
}

const List<Point>& SemesterResult::getPoints() const {
    return points_;
}
