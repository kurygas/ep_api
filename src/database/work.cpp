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
    json[Str::start] = getStart().toTime_t();
    json[Str::end] = getEnd().toTime_t();
    json[Str::subject] = static_cast<int>(getSubject());
    json[Str::semester] = getSemester();
    json[Str::workNumber] = getWorkNumber();
    json[Str::groupId] = getGroup().id();
    json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    return json;
}

const std::string& Work::getListName() {
    return Str::workList;
}

Work::Work(const Wt::WDateTime& start, const Wt::WDateTime& end, const Subject::Type subject, const int semester, 
    const int workNumber, const Ptr<Group>& group) {
    setTime(start, end);
    setSemester(semester);
    setWorkNumber(workNumber);
    setSubject(subject);
    setGroup(group);
}

void Work::setStart(const Wt::WDateTime& start) {
    if (start < Wt::WDateTime::currentDateTime() || start > end_) {
        throw BadRequestException("Invalid start for Work");
    }

    start_ = start;
}

void Work::setEnd(const Wt::WDateTime& end) {
    if (end < Wt::WDateTime::currentDateTime() || start_ > end) {
        throw BadRequestException("Invalid end for Work");
    }

    end_ = end;
}

void Work::setTime(const Wt::WDateTime& start, const Wt::WDateTime& end) {
    if (start > end || start < Wt::WDateTime::currentDateTime()) {
        throw BadRequestException("Invalid time segment for Work");
    }

    start_ = start;
    end_ = end;
}

void Work::setSemester(const int semester) {
    if (!Validator::isSemesterValid(semester)) {
        throw BadRequestException("Invalid semester for Work");
    }

    semester_ = semester;
}

void Work::setWorkNumber(const int workNumber) {
    if (!Validator::isWorkNumberValid(workNumber)) {
        throw BadRequestException("Invalid work_number for Work");
    }

    workNumber_ = workNumber;
}

void Work::setGroup(const Ptr<Group>& group) {
    group_ = group;
}

void Work::setProblems(const List<Problem>& problems) {
    for (const auto& problem : problems) {
        if (problems_.count(problem) == 0 && (!problem || problem->getSubject() != subject_ || problem->getSemester() != semester_ || 
            problem->getWorkNumber() != workNumber_)) {
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

const Wt::WDateTime& Work::getStart() const {
    return start_;
}

const Wt::WDateTime& Work::getEnd() const {
    return end_;
}

int Work::getSemester() const {
    return semester_;
}

int Work::getWorkNumber() const {
    return workNumber_;
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
