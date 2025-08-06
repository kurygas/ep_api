#include "work_result.h"
#include "http_exceptions.h"
#include "validator.h"
#include "random_functions.h"
#include "utility_functions.h"
#include "crypto.h"
#include "semester_result.h"
#include "problem.h"
#include "work.h"

WorkResult::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::workId] = getWork().id();
    json[Str::semesterResultId] = getSemesterResult().id();
    return json;
}

WorkResult::WorkResult(Ptr<Work> work, Ptr<SemesterResult> semesterResult) {
    auto problem = Random::pickRandom(work->getProblems());
    setWork(std::move(work));
    setProblem(std::move(problem));
    setSemesterResult(std::move(semesterResult));
}

void WorkResult::setFilename(std::string filename) {
    if (filename.empty()) {
        throw BadRequestException("Invalid filename for WorkResult");
    }

    if (!filename_.empty()) {
        throw BadRequestException("Another solution has been already pinned");
    }

    if (Wt::WDateTime::currentDateTime() > work_->getEnd()) {
        throw BadRequestException("Time is up");
    }

    filename_ = std::move(filename);
}

void WorkResult::setWork(Ptr<Work> work) {
    if (!work || semesterResult_ && work->getSemester() != semesterResult_->getSemester()) {
        throw BadRequestException("Invalid Work for WorkResult");
    }

    if (work->getProblems().empty()) {
        throw BadRequestException("No tasks in work");
    }

    if (work->getStart() > Wt::WDateTime::currentDateTime()) {
        throw BadRequestException("Not begin yet");
    }

    if (work->getEnd() < Wt::WDateTime::currentDateTime()) {
        throw BadRequestException("Already ended");
    }

    work_ = std::move(work);
}

void WorkResult::setProblem(Ptr<Problem> problem) {
    if (!problem || problem->getWorks().count(work_) == 0) {
        throw BadRequestException("Invalid Problem for WorkResult");
    }

    problem_ = std::move(problem);
}

void WorkResult::setSemesterResult(Ptr<SemesterResult> semesterResult) {
    if (!semesterResult || work_ && work_->getSemester() != semesterResult->getSemester()) {
        throw BadRequestException("Invalid SemesterResult for WorkResult");
    }

    semesterResult_ = std::move(semesterResult);
}

void WorkResult::setMark(const int mark) {
    if (mark < 0) {
        throw BadRequestException("Invalid mark for WorkResult");
    }

    mark_ = mark;
}

const std::string& WorkResult::getFilename() const {
    return filename_;
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
