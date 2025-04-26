#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/SqlTraits.h>
#include <Wt/WString.h>

class User;
class Problem;
class Work;

class WorkResult {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, filename_, "filename");
        Wt::Dbo::field(a, mark_, "mark");
        Wt::Dbo::field(a, queued_, "queued");
        Wt::Dbo::belongsTo(a, work_, "work");
        Wt::Dbo::belongsTo(a, problem_, "problem");
        Wt::Dbo::belongsTo(a, user_, "user");
    }

    WorkResult() = default;
    WorkResult(const Wt::Dbo::ptr<Work>& work, const Wt::Dbo::ptr<Problem>& problem, const Wt::Dbo::ptr<User>& user);

    void setFilename(const Wt::WString& filename);
    void setMark(int mark);

    const Wt::WString& getFilename() const;
    int getMark() const;
    const Wt::Dbo::ptr<Work>& getWork() const;
    const Wt::Dbo::ptr<Problem>& getProblem() const;
    const Wt::Dbo::ptr<User>& getUser() const;

private:
    void setWork(const Wt::Dbo::ptr<Work>& work);
    void setProblem(const Wt::Dbo::ptr<Problem>& problem);
    void setUser(const Wt::Dbo::ptr<User>& user);
    
    Wt::WString filename_;
    int mark_;
    bool queued_;

    Wt::Dbo::ptr<Work> work_;
    Wt::Dbo::ptr<Problem> problem_;
    Wt::Dbo::ptr<User> user_;
};
