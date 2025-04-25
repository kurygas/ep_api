#include "problem.h"
#include "checker.h"

Problem::Problem(const Wt::WString& name, const SubjectType subject, const unsigned semester, const unsigned workNumber) {
    setName(name);
    setSubject(subject);
    setSemester(semester);
    setWorkNumber(workNumber);
}

void Problem::setName(const Wt::WString &name) {
    if (name.empty()) {
        throw std::runtime_error("Invalid problem name");
    }

    name_ = name;
}

void Problem::setSubject(const SubjectType subject) {
    subject_ = subject;
}

void Problem::setSemester(const unsigned semester) {
    if (!isSemesterValid(semester)) {
        throw std::runtime_error("Invalid semester");
    }

    semester_ = semester;
}

void Problem::setWorkNumber(const unsigned workNumber) {
    workNumber_ = workNumber;
}

const Wt::WString& Problem::getName() const {
    return name_;
}

SubjectType Problem::getSubject() const {
    return subject_;
}

unsigned Problem::getSemester() const {
    return semester_;
}

unsigned Problem::getWorkNumber() const {
    return workNumber_;
}
