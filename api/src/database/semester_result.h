#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

class Semester;
class User;
class WorkResult;
class Point;

class SemesterResult {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::belongsTo(a, semester_);
        Wt::Dbo::belongsTo(a, user_);
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne);
        Wt::Dbo::hasMany(a, points_, Wt::Dbo::ManyToOne);
    }

    operator Wt::Json::Object() const;

    SemesterResult() = default;
    SemesterResult(Ptr<Semester> semester, Ptr<User> user);

    void setSemester(Ptr<Semester> semester);
    void setUser(Ptr<User> user);

    const Ptr<Semester>& getSemester() const;
    const Ptr<User>& getUser() const;
    const List<WorkResult>& getWorkResults() const;
    const List<Point>& getPoints() const;

private:
    Ptr<Semester> semester_;
    Ptr<User> user_;
    List<WorkResult> workResults_;
    List<Point> points_;
};
