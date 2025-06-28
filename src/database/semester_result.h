#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "subject_type.h"
#include "json.h"
#include "str.h"
#include "types.h"

class Semester;
class User;
class WorkResult;
class Point;

class SemesterResult {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::belongsTo(a, semester_, "semester");
        Wt::Dbo::belongsTo(a, user_, "user");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne, "semester_result");
        Wt::Dbo::hasMany(a, points_, Wt::Dbo::ManyToOne, "semester_result");
    }

    SemesterResult() = default;
    SemesterResult(const Ptr<Semester>& semester, const Ptr<User>& user);

    operator Wt::Json::Object() const;

    void setSemester(const Ptr<Semester>& semester);
    void setUser(const Ptr<User>& user);

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
