#include "work.h"
#include "validator.h"
#include "problem.h"
#include "group.h"
#include "work_result.h"
#include "http_exceptions.h"
#include "str.h"
#include "utility_functions.h"

Work::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();
    json[Str::start] = getStart().toTime_t();
    json[Str::end] = getEnd().toTime_t();
    json[Str::subject] = static_cast<int>(getSubject());
    json[Str::semesterNumber] = getSemesterNumber();
    json[Str::isExam] = isExam();
    json[Str::groupId] = getGroup().id();
    json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    return json;
}

Work::Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const Subject::Type subject, 
    const int semesterNumber, const Ptr<Group>& group, const bool isExam) {
    setName(name);
    setTime(start, end);
    setSubject(subject);
    setSemesterNumber(semesterNumber);
    setGroup(group);
    setIsExam(isExam);
}

void Work::setName(const Wt::WString& name) {
    if (name.empty()) {
        throw BadRequestException("Invalid name for Work");
    }

    name_ = name;
}

void Work::setStart(const Wt::WDateTime& start) {
    if (Validator::isTimeSegmentValid(start, end_)) {
        throw BadRequestException("Invalid start for Work");
    }

    start_ = start;
}

void Work::setEnd(const Wt::WDateTime& end) {
    if (Validator::isTimeSegmentValid(start_, end)) {
        throw BadRequestException("Invalid end for Work");
    }

    end_ = end;
}

void Work::setTime(const Wt::WDateTime& start, const Wt::WDateTime& end) {
    if (Validator::isTimeSegmentValid(start, end)) {
        throw BadRequestException("Invalid time segment for Work");
    }

    start_ = start;
    end_ = end;
}

void Work::setSemesterNumber(const int semesterNumber) {
    if (!Validator::isSemesterNumberValid(semesterNumber)) {
        throw BadRequestException("Invalid semester_number for Work");
    }

    semesterNumber_ = semesterNumber;
}

void Work::setGroup(const Ptr<Group>& group) {
    if (!group) {
        throw BadRequestException("Invalid Group for Work");
    }

    group_ = group;
}

void Work::setProblems(const List<Problem>& problems) {
    for (const auto& problem : problems) {
        if (problems_.count(problem) == 0 && problem->getSubject() != subject_) {
            throw BadRequestException("Invalid problems for work");
        } 
    }

    for (const auto& problem : problems_) {
        if (problems_.count(problem) == 0) {
            for (const auto& workResult : workResults_) {
                if (workResult->getProblem() == problem) {
                    throw BadRequestException("Invalid problems for work");
                }
            }
        }
    }

    problems_ = problems;
}

void Work::setSubject(const Subject::Type subject) {
    subject_ = subject;
}

void Work::setIsExam(const bool isExam) {
    isExam_ = isExam;
}

const Wt::WString& Work::getName() const {
    return name_;
}

const Wt::WDateTime& Work::getStart() const {
    return start_;
}

const Wt::WDateTime& Work::getEnd() const {
    return end_;
}

int Work::getSemesterNumber() const {
    return semesterNumber_;
}

const Ptr<Group> Work::getGroup() const {
    return group_;
}

const List<Problem>& Work::getProblems() const {
    return problems_;
}

const List<WorkResult>& Work::getWorkResults() const {
    return workResults_;
}

Subject::Type Work::getSubject() const {
    return subject_;
}

bool Work::isExam() const {
    return isExam_;
}
