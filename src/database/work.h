#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"
#include "str.h"
#include "types.h"

class Problem;
class WorkResult;
class Semester;

class Work {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, start_, Str::start);
        Wt::Dbo::field(a, end_, Str::end);
        Wt::Dbo::field(a, isExam_, Str::isExam);
        Wt::Dbo::belongsTo(a, semester_, "semester");
        Wt::Dbo::hasMany(a, problems_, Wt::Dbo::ManyToMany, "work_problems");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "work");
    }

    operator Wt::Json::Object() const;

    Work() = default;
    Work(const Wt::WString& name, const Wt::WDateTime& start, const Wt::WDateTime& end, const Ptr<Semester>& semester, bool isExam);

    void setName(const Wt::WString& name);
    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setTime(const Wt::WDateTime& start, const Wt::WDateTime& end);
    void setSemester(const Ptr<Semester>& semester);
    void setProblems(const List<Problem>& problems);
    void setIsExam(bool isExam);

    const Wt::WString& getName() const;
    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    bool isExam() const;
    const Ptr<Semester>& getSemester() const;
    const List<Problem>& getProblems() const;
    const List<WorkResult>& getWorkResults() const;

private:
    Wt::WString name_;
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    bool isExam_;

    Ptr<Semester> semester_;
    List<Problem> problems_;
    List<WorkResult> workResults_;
};
