#include "work.h"
#include "validator.h"
#include "problem.h"
#include "semester.h"
#include "work_result.h"
#include "http_exceptions.h"
#include "str.h"
#include "utility_functions.h"

Work::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::name] = getName();
    json[Str::start] = getStart().toTime_t();
    json[Str::end] = getEnd().toTime_t();
    json[Str::semesterId] = getSemester().id();
    json[Str::isExam] = isExam();
    json[Str::workResultList] = JsonFunctions::getIdArray(getWorkResults());
    return json;
}

Work::Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const Ptr<Semester>& semester, 
    const bool isExam) {
    setName(name);
    setTime(start, end);
    setSemester(semester);
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

void Work::setSemester(const Ptr<Semester>& semester) {
    if (!semester) {
        throw BadRequestException("Invalid semester for Work");
    }

    semester_ = semester;
}

void Work::setProblems(const List<Problem>& problems) {
    for (const auto& problem : problems) {
        if (problems_.count(problem) == 0 && problem->getSubject() != semester_->getSubject()) {
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

const Ptr<Semester>& Work::getSemester() const {
    return semester_;
}

const List<Problem>& Work::getProblems() const {
    return problems_;
}

const List<WorkResult>& Work::getWorkResults() const {
    return workResults_;
}

bool Work::isExam() const {
    return isExam_;
}
