#include "semester_result.h"
#include "semester.h"
#include "user.h"
#include "work_result.h"
#include "point.h"
#include "http_exceptions.h"

SemesterResult::SemesterResult(const Ptr<Semester>& semester, const Ptr<User>& user) {
    setSemester(semester);
    setUser(user);
}

SemesterResult::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::semesterId] = getSemester().id();
    json[Str::userId] = getSemester().id();
    json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    json[Str::pointList] = JsonFunctions::getIdArray(getPoints());
    return json;
}

void SemesterResult::setSemester(const Ptr<Semester>& semester) {
    if (!semester || user_ && semester->getGroup() != user_->getGroup()) {
        throw BadRequestException("Invalid Semester for SemesterResult");
    }

    semester_ = semester;
}

void SemesterResult::setUser(const Ptr<User>& user) {
    if (!user || semester_ && semester_->getGroup() != user->getGroup()) {
        throw BadRequestException("Invalid User for SemesterResult");
    }

    user_ = user;
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
