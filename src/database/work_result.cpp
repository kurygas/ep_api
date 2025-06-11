#include "work_result.h"
#include "user.h"
#include "problem.h"
#include "work.h"
#include "http_exceptions.h"
#include "str.h"

WorkResult::operator Wt::Json::Object() const {
    Wt::Json::Object json;
    json[Str::filename] = getFilename();
    json[Str::mark] = getMark();
    json[Str::queued] = isQueued();
    json[Str::workId] = getWork().id();
    json[Str::problemId] = getProblem().id();
    json[Str::userId] = getUser().id();
    return json;
}

WorkResult::WorkResult(const Wt::Dbo::ptr<Work>& work, const Wt::Dbo::ptr<Problem>& problem, const Wt::Dbo::ptr<User>& user)
: queued_(true) {
    setWork(work);
    setProblem(problem);
    setUser(user);
}

void WorkResult::setFilename(const Wt::WString& filename) {
    if (filename.empty() || !filename_.empty()) {
        throw BadRequestException("Invalid filename for WorkResult");
    }

    filename_ = filename;
}

void WorkResult::setMark(int mark) {
    if (mark < 0) {
        throw BadRequestException("Invalid mark for WorkResult");
    }

    queued_ = false;
    mark_ = mark;
}

const Wt::WString& WorkResult::getFilename() const {
    return filename_;
}

int WorkResult::getMark() const {
    return mark_;
}

bool WorkResult::isQueued() const {
    return queued_;
}

const Wt::Dbo::ptr<Work>& WorkResult::getWork() const {
    return work_;
}

const Wt::Dbo::ptr<Problem>& WorkResult::getProblem() const {
    return problem_;
}

const Wt::Dbo::ptr<User>& WorkResult::getUser() const {
    return user_;
}

void WorkResult::setWork(const Wt::Dbo::ptr<Work>& work) {
    if (!work) {
        throw BadRequestException("Invalid work for WorkResult");
    }

    work_ = work;
}

void WorkResult::setProblem(const Wt::Dbo::ptr<Problem>& problem) {
    if (!problem || problem->getWorks().count(work_) == 0) {
        throw BadRequestException("Invalid problem for WorkResult");
    }

    problem_ = problem;
}

void WorkResult::setUser(const Wt::Dbo::ptr<User>& user) {
    if (!user) {
        throw BadRequestException("Invalid user for WorkResult");
    }

    user_ = user;
}

void WorkResult::setQueued(const bool queued) {
    queued_ = queued;
}
