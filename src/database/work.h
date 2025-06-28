#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"
#include "str.h"
#include "types.h"

class Group;
class Problem;
class WorkResult;

class Work {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, start_, Str::start);
        Wt::Dbo::field(a, end_, Str::end);
        Wt::Dbo::field(a, subject_, Str::subject);
        Wt::Dbo::field(a, semesterNumber_, Str::semesterNumber);
        Wt::Dbo::field(a, isExam_, Str::isExam);
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, problems_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "work");
    }

    operator Wt::Json::Object() const;

    Work() = default;
    Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, Subject::Type subject, int semesterNumber, 
        const Ptr<Group>& group, bool isExam);

    void setName(const Wt::WString& name);
    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setTime(const Wt::WDateTime& start, const Wt::WDateTime& end);
    void setSubject(Subject::Type subject);
    void setSemesterNumber(int semesterNumber);
    void setGroup(const Ptr<Group>& group);
    void setProblems(const List<Problem>& problems);
    void setIsExam(bool isExam);

    const Wt::WString& getName() const;
    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    Subject::Type getSubject() const;
    int getSemesterNumber() const;
    bool isExam() const;
    const Ptr<Group> getGroup() const;
    const List<Problem>& getProblems() const;
    const List<WorkResult>& getWorkResults() const;

private:
    Wt::WString name_;
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    Subject::Type subject_;
    int semesterNumber_;
    bool isExam_;

    Ptr<Group> group_;
    List<Problem> problems_;
    List<WorkResult> workResults_;
};
