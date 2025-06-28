#include "problem.h"
#include "validator.h"
#include "work.h"
#include "work_result.h"
#include "http_exceptions.h"
#include "str.h"

Problem::Problem(const Wt::WString& name, const Wt::WString& statement, Subject::Type subject, const int semesterNumber) {
    setName(name);
    setStatement(statement);
    setSubject(subject);
    setSemesterNumber(semesterNumber);
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

void Problem::setSemesterNumber(const int semesterNumber) {
    if (!Validator::isSemesterNumberValid(semesterNumber)) {
        throw BadRequestException("Invalid semester for Problem");
    }

    semesterNumber_ = semesterNumber;
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

int Problem::getSemesterNumber() const {
    return semesterNumber_;
}

const List<Work>& Problem::getWorks() const {
    return works_;
}

const List<WorkResult>& Problem::getWorkResults() const {
    return workResults_;
}

Problem::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();
    json[Str::statement] = getStatement();
    json[Str::subject] = static_cast<int>(getSubject());
    json[Str::semesterNumber] = getSemesterNumber();
    json[Str::workList] = JsonFunctions::getIdArray(getWorks());
    json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    return json;
}
