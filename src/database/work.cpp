#include "work.h"
#include "checker.h"

Work::Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const unsigned semester, const unsigned workNumber)
: start_(Wt::WDateTime::currentDateTime())
, end_(Wt::WDateTime::currentDateTime()) {
    setName(name);
    setStart(start);
    setEnd(end);
    setSemester(semester);
    setWorkNumber(workNumber);
}

void Work::setName(const Wt::WString &name)
{
    if (name.empty()) {
        throw std::runtime_error("Invalid work name");
    }

    name_ = name;
}

void Work::setStart(const Wt::WDateTime& start) {
    if (start < Wt::WDateTime::currentDateTime() || start > end_) {
        throw std::runtime_error("Invalid work start time");
    }

    start_ = start;
}

void Work::setEnd(const Wt::WDateTime& end) {
    if (end < Wt::WDateTime::currentDateTime() || start_ > end) {
        throw std::runtime_error("Invalid work end time");
    }

    end_ = end;
}

void Work::setSemester(const unsigned semester) {
    if (!isSemesterValid(semester)) {
        throw std::runtime_error("Invalid semester");
    }

    semester_ = semester;
}

void Work::setWorkNumber(const unsigned workNumber) {
    workNumber_ = workNumber;
}

void Work::setGroup(const Wt::Dbo::ptr<Group>& group) {
    group_ = group;
}

void Work::addProblem(const Wt::Dbo::ptr<Problem>& problem) {
    problemSet_.insert(problem);
}

void Work::removeProblem(const Wt::Dbo::ptr<Problem>& problem) {
    problemSet_.erase(problem);
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

unsigned Work::getSemester() const {
    return semester_;
}

unsigned Work::getWorkNumber() const {
    return workNumber_;
}

const Wt::Dbo::ptr<Group> Work::getGroup() const {
    return group_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>> &Work::getProblemSet() const {
    problemSet_;
}
