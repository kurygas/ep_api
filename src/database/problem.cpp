#include "problem.h"
#include "checker.h"
#include "work.h"
#include "work_result.h"

Problem::Problem(const Wt::WString& name, const Wt::WString& statement, const SubjectType subject, const int semester, const int workNumber) {
    setName(name);
    setStatement(statement);
    setSubject(subject);
    setSemester(semester);
    setWorkNumber(workNumber);
}

void Problem::setName(const Wt::WString &name) {
    if (name.empty()) {
        throw std::runtime_error("Invalid name for Problem");
    }

    name_ = name;
}

void Problem::setStatement(const Wt::WString& statement) {
    if (statement.empty()) {
        throw std::runtime_error("Invalid statement for Problem");
    }

    statement_ = statement;
}

void Problem::setSubject(const SubjectType subject) {
    subject_ = subject;
}

void Problem::setSemester(const int semester) {
    if (!isSemesterValid(semester)) {
        throw std::runtime_error("Invalid semester for Problem");
    }

    semester_ = semester;
}

void Problem::setWorkNumber(const int workNumber) {
    if (!isWorkNumberValid(workNumber)) {
        throw std::runtime_error("Invalid work_number for Problem");
    }

    workNumber_ = workNumber;
}

const Wt::WString& Problem::getName() const {
    return name_;
}

const Wt::WString& Problem::getStatement() const {
    return statement_;
}

SubjectType Problem::getSubject() const {
    return subject_;
}

int Problem::getSemester() const {
    return semester_;
}

int Problem::getWorkNumber() const {
    return workNumber_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& Problem::getWorks() const {
    return works_;
}

const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& Problem::getWorkResults() const {
    return workResults_;
}
