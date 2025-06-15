#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"
#include "str.h"

class Group;
class Problem;
class WorkResult;

class Work {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, start_, Str::start);
        Wt::Dbo::field(a, end_, Str::end);
        Wt::Dbo::field(a, subject_, Str::subject);
        Wt::Dbo::field(a, semester_, Str::semester);
        Wt::Dbo::field(a, workNumber_, Str::workNumber);
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, problems_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "work");
    }

    operator Wt::Json::Object() const;
    static const std::string& getListName();

    Work() = default;
    Work(const Wt::WDateTime& start, const Wt::WDateTime& end, Subject::Type subject, int semester, int workNumber, 
        const Wt::Dbo::ptr<Group>& group);

    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setTime(const Wt::WDateTime& start, const Wt::WDateTime& end);
    void setSubject(Subject::Type subject);
    void setSemester(int semester);
    void setWorkNumber(int workNumber);
    void setGroup(const Wt::Dbo::ptr<Group>& group);
    void setProblems(const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>>& problems);

    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    Subject::Type getSubject() const;
    int getSemester() const;
    int getWorkNumber() const;
    const Wt::Dbo::ptr<Group> getGroup() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<Problem>>& getProblems() const;
    const Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>>& getWorkResults() const;

private:
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    Subject::Type subject_;
    int semester_;
    int workNumber_;

    Wt::Dbo::ptr<Group> group_;
    Wt::Dbo::collection<Wt::Dbo::ptr<Problem>> problems_;
    Wt::Dbo::collection<Wt::Dbo::ptr<WorkResult>> workResults_;
};
