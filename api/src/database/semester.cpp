#include "semester.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "crypto.h"
#include "group.h"
#include "semester_result.h"

Semester::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::semesterNumber] = getSemesterNumber();
    json[Str::subject] = static_cast<int>(getSubject());
    json[Str::start] = getStart().toTime_t();
    json[Str::end] = getEnd().toTime_t();
    json[Str::groupId] = getGroup().id();

    if (!getSemesterResults().empty()) {
        json[Str::semesterResultList] = JsonFunctions::getIdArray(getSemesterResults());
    }

    if (getCfMaxPoint() > 0) {
        json[Str::cfMaxPoint] = getCfMaxPoint();
    }

    if (getAtcRatio() > 0) {
        json[Str::atcRatio] = getAtcRatio();
    }
    
    return json;
}

Semester::Semester(const int semesterNumber, const Subject::Type subject, const Wt::WDateTime& start, const Wt::WDateTime& end, 
    Ptr<Group> group) {
    setSemesterNumber(semesterNumber);
    setSubject(subject);
    setTime(start, end);
    setGroup(std::move(group));
}

void Semester::setGroup(Ptr<Group> group) {
    if (!group) {
        throw BadRequestException("Invalid Group for Semester");
    }
    
    group_ = std::move(group);
}

void Semester::setSemesterNumber(const int semesterNumber) {
    if (!Validator::isSemesterNumberValid(semesterNumber)) {
        throw BadRequestException("Invalid semester for SemesterResult");
    }

    semesterNumber_ = semesterNumber;
}

void Semester::setSubject(const Subject::Type subject) {
    subject_ = subject;
}

void Semester::setStart(const Wt::WDateTime& start) {
    if (!Validator::isTimeSegmentValid(start, end_)) {
        throw BadRequestException("Invalid start for SemesterResult");
    }

    start_ = start;
}

void Semester::setEnd(const Wt::WDateTime& end) {
    if (!Validator::isTimeSegmentValid(start_, end)) {
        throw BadRequestException("Invalid end for SemesterResult");
    }

    end_ = end;
}

void Semester::setTime(const Wt::WDateTime& start, const Wt::WDateTime& end) {
    if (!Validator::isTimeSegmentValid(start, end)) {
        throw BadRequestException("Invalid time segment for SemesterResult");
    }

    start_ = start;
    end_ = end;
}

int Semester::getSemesterNumber() const {
    return semesterNumber_;
}

Subject::Type Semester::getSubject() const {
    return subject_;
}

const Wt::WDateTime& Semester::getStart() const {
    return start_;
}

const Wt::WDateTime& Semester::getEnd() const {
    return end_;
}

const Ptr<Group> Semester::getGroup() const {
    return group_;
}

const List<SemesterResult>& Semester::getSemesterResults() const {
    return semesterResults_;
}

int Semester::getCfMaxPoint() const {
    return cfMaxPoint_;
}

int Semester::getAtcRatio() const {
    return atcRatio_;
}

void Semester::setCfMaxPoint(const int cfMaxPoint) {
    if (subject_ != Subject::Type::Algo) {
        throw BadRequestException("Invalid subject");
    }

    if (cfMaxPoint < 0) {
        throw BadRequestException("Invalid cf_max_point for Group");
    }

    cfMaxPoint_ = cfMaxPoint;
}

void Semester::setAtcRatio(const int atcRatio) {
    if (subject_ != Subject::Type::Algo) {
        throw BadRequestException("Invalid subject");
    }

    if (atcRatio <= 0) {
        throw BadRequestException("Invalid atc_max_point");
    }

    atcRatio_ = atcRatio;
}
