#include "work_result.h"
#include "user.h"
#include "problem.h"
#include "work.h"
#include "http_exceptions.h"
#include "str.h"
#include "random_functions.h"
#include "point.h"
#include "semester_result.h"
#include "semester.h"

WorkResult::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::workId] = getWork().id();
    json[Str::mark] = getMark();
    json[Str::semesterResultId] = getSemesterResult().id();
    return json;
}

WorkResult::WorkResult(const Ptr<Work>& work, const Ptr<SemesterResult>& semesterResult) {
    if (work->getProblems().empty()) {
        throw BadRequestException("No tasks in work");
    }

    if (work->getStart() > Wt::WDateTime::currentDateTime()) {
        throw BadRequestException("Not begin yet");
    }

    if (work->getEnd() < Wt::WDateTime::currentDateTime()) {
        throw BadRequestException("Already ended");
    }

    if (work->getSubject() != semesterResult->getSemester()->getSubject()) {
        throw BadRequestException("Invalid SemesterResult for Work");
    }

    setWork(work);
    setProblem(Random::pickRandom(work->getProblems()));
    setSemesterResult(semesterResult);
}

void WorkResult::setFilename(const Wt::WString& filename) {
    if (filename.empty()) {
        throw BadRequestException("Invalid filename for WorkResult");
    }

    if (!filename_.empty()) {
        throw BadRequestException("Another solution has been already pinned");
    }

    if (Wt::WDateTime::currentDateTime() > work_->getEnd()) {
        throw BadRequestException("Time is up");
    }

    filename_ = filename;
}

void WorkResult::setMark(const int mark) {
    if (mark < 0) {
        throw BadRequestException("Invalid mark for WorkResult");
    }

    mark_ = mark;
}

const Wt::WString& WorkResult::getFilename() const {
    return filename_;
}

std::string WorkResult::getSolutionPath() const {
    if (filename_.empty()) {
        throw NotFoundException("No solution in work result");
    }

    auto path = Str::solutionsPath;
    path += std::to_string(getSemesterResult()->getUser()->getTgId()) + '/';
    path += std::to_string(static_cast<int>(work_->getSubject())) + '/';
    path += std::to_string(getSemesterResult()->getSemester()->getSemesterNumber()) + '/';
    path += getWork()->getName().toUTF8() + '/';
    path += filename_.toUTF8();
    return path;
}

int WorkResult::getMark() const {
    return mark_;
}

const Ptr<Work>& WorkResult::getWork() const {
    return work_;
}

const Ptr<Problem>& WorkResult::getProblem() const {
    return problem_;
}

const Ptr<SemesterResult>& WorkResult::getSemesterResult() const {
    return semesterResult_;
}

void WorkResult::setWork(const Ptr<Work>& work) {
    if (!work) {
        throw BadRequestException("Invalid Work for WorkResult");
    }

    work_ = work;
}

void WorkResult::setProblem(const Ptr<Problem>& problem) {
    if (!problem || problem->getWorks().count(work_) == 0) {
        throw BadRequestException("Invalid Problem for WorkResult");
    }

    problem_ = problem;
}

void WorkResult::setSemesterResult(const Ptr<SemesterResult>& semesterResult) {
    if (!semesterResult) {
        throw BadRequestException("Invalid SemesterResult for WorkResult");
    }

    semesterResult_ = semesterResult;
}
