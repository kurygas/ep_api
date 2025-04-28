#include "problem.h"
#include "checker.h"
#include "work.h"
#include "work_result.h"
#include "http_exceptions.h"

Problem::Problem(const Wt::WString& name, const Wt::WString& statement, const Subject::Type subject, const int semester, const int workNumber) {
    setName(name);
    setStatement(statement);
    setSubject(subject);
    setSemester(semester);
    setWorkNumber(workNumber);
}

void Problem::setName(const Wt::WString& name) {
    if (name.empty()) {
        throw BadRequestException("Invalid name for Problem");
    }

    name_ = name;
}

void Problem::setStatement(const Wt::WString& statement) {
    if (statement.empty()) {
        throw BadRequestException("Invalid statement for Problem");
    }

    statement_ = statement;
}

void Problem::setSubject(const Subject::Type subject) {
    subject_ = subject;
}

void Problem::setSemester(const int semester) {
    if (!isSemesterValid(semester)) {
        throw BadRequestException("Invalid semester for Problem");
    }

    semester_ = semester;
}

void Problem::setWorkNumber(const int workNumber) {
    if (!isWorkNumberValid(workNumber)) {
        throw BadRequestException("Invalid work_number for Problem");
    }

    workNumber_ = workNumber;
}

const Wt::WString& Problem::getName() const {
    return name_;
}

const Wt::WString& Problem::getStatement() const {
    return statement_;
}

Subject::Type Problem::getSubject() const {
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
