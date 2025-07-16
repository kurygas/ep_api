#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

class Group;
class SemesterResult;

class Semester {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, semesterNumber_, Str::semesterNumber);
        Wt::Dbo::field(a, start_, Str::start);
        Wt::Dbo::field(a, end_, Str::end);
        Wt::Dbo::field(a, subject_, Str::subject);
        Wt::Dbo::field(a, cfMaxPoint_, Str::cfMaxPoint);
        Wt::Dbo::field(a, atcRatio_, Str::atcRatio);
        Wt::Dbo::belongsTo(a, group_, "group");
        Wt::Dbo::hasMany(a, semesterResults_, Wt::Dbo::ManyToOne, "semester");
    }

    operator Wt::Json::Object() const;

    Semester() = default;
    Semester(int semesterNumber, Subject::Type subject, const Wt::WDateTime& start, const Wt::WDateTime& end, Ptr<Group> group);

    void setGroup(Ptr<Group> group);
    void setSemesterNumber(int semesterNumber);
    void setSubject(Subject::Type subject);
    void setStart(const Wt::WDateTime& start);
    void setEnd(const Wt::WDateTime& end);
    void setTime(const Wt::WDateTime& start, const Wt::WDateTime& end);
    void setCfMaxPoint(int cfMaxPoint);
    void setAtcRatio(int atcRatio);

    int getSemesterNumber() const;
    Subject::Type getSubject() const;
    const Wt::WDateTime& getStart() const;
    const Wt::WDateTime& getEnd() const;
    const Ptr<Group> getGroup() const;
    const List<SemesterResult>& getSemesterResults() const;
    int getCfMaxPoint() const;
    int getAtcRatio() const;

private:
    int semesterNumber_;
    Subject::Type subject_;
    Wt::WDateTime start_;
    Wt::WDateTime end_;
    int cfMaxPoint_ = 0;
    int atcRatio_ = 0;

    Ptr<Group> group_;
    List<SemesterResult> semesterResults_;
};
