#include "work.h"
#include "checker.h"
#include "problem.h"
#include "group.h"
#include "work_result.h"

Work::Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const SubjectType subject, 
    const int semester, const int workNumber)
: start_(Wt::WDateTime::currentDateTime())
, end_(Wt::WDateTime::currentDateTime()) {
    setName(name);
    setStart(start);
    setEnd(end);
    setSemester(semester);
    setWorkNumber(workNumber);
    setSubject(subject);
}

void Work::setName(const Wt::WString &name) {
    if (name.empty()) {
        throw std::runtime_error("Invalid name for Work");
    }

    name_ = name;
}

void Work::setStart(const Wt::WDateTime& start) {
    if (start < Wt::WDateTime::currentDateTime() || start > end_) {
        throw std::runtime_error("Invalid start for Work");
    }

    start_ = start;
}

void Work::setEnd(const Wt::WDateTime& end) {
    if (end < Wt::WDateTime::currentDateTime() || start_ > end) {
        throw std::runtime_error("Invalid end for Work");
    }

    end_ = end;
}

void Work::setSemester(const int semester) {
    if (!isSemesterValid(semester)) {
        throw std::runtime_error("Invalid semester for Work");
    }

    semester_ = semester;
}

void Work::setWorkNumber(const int workNumber) {
    if (!isWorkNumberValid(workNumber)) {
        throw std::runtime_error("Invalid work_number for Work");
    }

    workNumber_ = workNumber;
}

void Work::setGroup(const Wt::Dbo::ptr<Group>& group) {
    group_ = group;
}

void Work::addProblem(const Wt::Dbo::ptr<Problem>& problem) {
    if (!problem || problem->getSubject() != subject_ || problem->getSemester() != semester_ || problem->getWorkNumber() != workNumber_) {
        throw std::runtime_error("Invalid problem for Work");
    }

    problemSet_.insert(problem);
}

void Work::removeProblem(const Wt::Dbo::ptr<Problem>& problem) {
    problemSet_.erase(problem);
}

void Work::setSubject(const SubjectType subject) {
    subject_ = subject;
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

int Work::getSemester() const {
    return semester_;
}

int Work::getWorkNumber() const {
    return workNumber_;
}

const Wt::Dbo::ptr<Group> Work::getGroup() const {
    return group_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>>& Work::getProblemSet() const {
    return problemSet_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& Work::getWorkResults() const {
    return workResults_;
}

SubjectType Work::getSubject() const {
    return subject_;
}
