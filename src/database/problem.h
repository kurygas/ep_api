#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"

class Work;
class WorkResult;

class Problem {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, "name");
        Wt::Dbo::field(a, statement_, "statement");
        Wt::Dbo::field(a, subject_, "subject");
        Wt::Dbo::field(a, semester_, "semester");
        Wt::Dbo::field(a, workNumber_, "work_number");
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "problem");
    }

    Problem() = default;
    Problem(const Wt::WString& name, const Wt::WString& statement, SubjectType subject, int semester, int workNumber);

    void setName(const Wt::WString& name);
    void setStatement(const Wt::WString& statement);
    void setSubject(SubjectType subject);
    void setSemester(int semester);
    void setWorkNumber(int workNumber);

    const Wt::WString& getName() const;
    const Wt::WString& getStatement() const;
    SubjectType getSubject() const;
    int getSemester() const;
    int getWorkNumber() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Work>>& getWorks() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;

private:
    Wt::WString name_;
    Wt::WString statement_;
    SubjectType subject_;
    int semester_;
    int workNumber_;

    Wt::Dbo::collection<Wt::Dbo::ptr<Work>> works_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
};