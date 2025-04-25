#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"

class Work;

class Problem {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, "name");
        Wt::Dbo::field(a, subjectType_, "subject");
        Wt::Dbo::field(a, semester_, "semester");
        Wt::Dbo::field(a, workNumber_, "work_number");
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToMany, "work_problems");
    }

    Problem(const Wt::WString& name, SubjectType subject, unsigned semester, unsigned workNumber);

    void setName(const Wt::WString& name);
    void setSubject(SubjectType subject);
    void setSemester(unsigned semester);
    void setWorkNumber(unsigned workNumber);

    const Wt::WString& getName() const;
    SubjectType getSubject() const;
    unsigned getSemester() const;
    unsigned getWorkNumber() const;

private:
    Wt::WString name_;
    SubjectType subject_;
    unsigned semester_;
    unsigned workNumber_;

    Wt::Dbo::collection<Wt::Dbo::ptr<Work>> works_;
};