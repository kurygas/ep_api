#include "work_result.h"
#include "user.h"
#include "problem.h"
#include "work.h"

WorkResult::WorkResult(const Wt::Dbo::ptr<Work>& work, const Wt::Dbo::ptr<Problem>& problem, const Wt::Dbo::ptr<User>& user)
: queued_(true) {
    setWork(work);
    setProblem(problem);
    setUser(user);
}

void WorkResult::setFilename(const Wt::WString &filename)
{
    if (filename_.empty()) {
        throw std::runtime_error("Invalid filename for WorkResult");
    }

    filename_ = filename;
}

void WorkResult::setMark(int mark) {
    if (mark < 0) {
        throw std::runtime_error("Invalid mark for WorkResult");
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
        throw std::runtime_error("Invalid work for WorkResult");
    }

    work_ = work;
}

void WorkResult::setProblem(const Wt::Dbo::ptr<Problem>& problem) {
    if (!problem || problem->getWorks().count(work_) == 0) {
        throw std::runtime_error("Invalid problem for WorkResult");
    }

    problem_ = problem;
}

void WorkResult::setUser(const Wt::Dbo::ptr<User>& user) {
    // can we compare pointers here?

    if (!user || user->getGroup() != work_->getGroup()) {
        throw std::runtime_error("Invalid user for WorkResult");
    }

    user_ = user;
}
