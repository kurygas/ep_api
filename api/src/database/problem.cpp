#include "problem.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "work.h"
#include "work_result.h"

void Problem::setSubject(const Subject::Type subject) {
    subject_ = subject;
}

Problem::Problem(std::string name, std::string statement, Subject::Type subject, int semesterNumber) {
    setName(std::move(name));
    setStatement(std::move(statement));
    setSubject(subject);
    setSemesterNumber(semesterNumber);
}

void Problem::setName(std::string name)
{
    if (name.empty()) {
        throw BadRequestException("Invalid name for Problem");
    }

    name_ = std::move(name);
}

void Problem::setStatement(std::string statement) {
    if (statement.empty()) {
        throw BadRequestException("Invalid statement for Problem");
    }

    statement_ = std::move(statement);
}

void Problem::setSemesterNumber(const int semesterNumber) {
    if (!Validator::isSemesterNumberValid(semesterNumber)) {
        throw BadRequestException("Invalid semester for Problem");
    }

    semesterNumber_ = semesterNumber;
}

const std::string& Problem::getName() const {
    return name_;
}

const std::string& Problem::getStatement() const {
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
    json[Str::name] = getName().c_str();
    json[Str::statement] = getStatement().c_str();
    json[Str::subject] = static_cast<int>(getSubject());
    json[Str::semesterNumber] = getSemesterNumber();

    if (!getWorks().empty()) {
        json[Str::workList] = JsonFunctions::getIdArray(getWorks());
    }

    if (!getWorkResults().empty()) {
        json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    }
    
    return json;
}
