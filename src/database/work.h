#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"

class Group;
class User;
class Problem;

class Work {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, "name");
        Wt::Dbo::field(a, start_, "start");
        Wt::Dbo::field(a, end_, "end");
        Wt::Dbo::field(a, semester_, "semester");
        Wt::Dbo::field(a, workNumber_, "work_number");
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, problemSet_, Wt::Dbo::ManyToMany, "work_problems");
    }

    Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, unsigned semester, unsigned workNumber);

    void setName(const Wt::WString& name);
    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setSemester(unsigned semester);
    void setWorkNumber(unsigned workNumber);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void addProblem(const Wt::Dbo::ptr<Problem>& problem);
    void removeProblem(const Wt::Dbo::ptr<Problem>& problem);

    const Wt::WString& getName() const;
    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    unsigned getSemester() const;
    unsigned getWorkNumber() const;
    const Wt::Dbo::ptr<Group> getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>>& getProblemSet() const;

private:
    Wt::WString name_;
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    unsigned semester_;
    unsigned workNumber_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Problem>> problemSet_;
};