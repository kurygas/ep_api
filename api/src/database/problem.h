#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>

#include "json.h"
#include "str.h"
#include "types.h"
#include "subject_type.h"

class Work;
class WorkResult;

class Problem {
public:
    template<typename Action>
    void persist(Action& a) {
        Wt::Dbo::field(a, name_, Str::name);
        Wt::Dbo::field(a, statement_, Str::statement);
        Wt::Dbo::field(a, subject_, Str::subject);
        Wt::Dbo::field(a, semesterNumber_, Str::semesterNumber);
        Wt::Dbo::hasMany(a, works_, Wt::Dbo::ManyToMany, "work_problem");
        Wt::Dbo::hasMany(a, workResults_, Wt::Dbo::ManyToOne);
    }

    operator Wt::Json::Object() const;

    Problem() = default;
    Problem(std::string name, std::string statement, Subject::Type subject, int semesterNumber);

    void setName(std::string name);
    void setStatement(std::string statement);
    void setSubject(Subject::Type subject);
    void setSemesterNumber(int semesterNumber);

    const std::string& getName() const;
    const std::string& getStatement() const;
    Subject::Type getSubject() const;
    int getSemesterNumber() const;
    const List<Work>& getWorks() const;
    const List<WorkResult>& getWorkResults() const;

private:
    std::string name_;
    std::string statement_;
    Subject::Type subject_;
    int semesterNumber_;

    List<Work> works_;
    List<WorkResult> workResults_;
};
