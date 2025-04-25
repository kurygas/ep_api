#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"

class Group;
class Problem;
class WorkResult;

class Work {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, "name");
        Wt::Dbo::field(a, start_, "start");
        Wt::Dbo::field(a, end_, "end");
        Wt::Dbo::field(a, subject_, "subject");
        Wt::Dbo::field(a, semester_, "semester");
        Wt::Dbo::field(a, workNumber_, "work_number");
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, problemSet_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "work");
    }

    Work() = default;
    Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, SubjectType subject, int semester, 
        int workNumber);

    void setName(const Wt::WString& name);
    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setSubject(SubjectType subject);
    void setSemester(int semester);
    void setWorkNumber(int workNumber);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void addProblem(const Wt::Dbo::ptr<Problem>& problem);
    void removeProblem(const Wt::Dbo::ptr<Problem>& problem);

    const Wt::WString& getName() const;
    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    SubjectType getSubject() const;
    int getSemester() const;
    int getWorkNumber() const;
    const Wt::Dbo::ptr<Group> getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>>& getProblemSet() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;

private:
    Wt::WString name_;
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    SubjectType subject_;
    int semester_;
    int workNumber_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Problem>> problemSet_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
};